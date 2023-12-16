// Copyright Sergio Calahorro

#include "Player/AuraPlayerController.h"

// Headers - Unreal Engine
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/InteractableInterface.h"
#include "UI/Widget/DamageTextWidgetComponent.h"
#include "Actor/Spells/MagicCircle.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation() */
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace(DeltaTime);
	AutoRun();
	UpdateMagicCircleLocation();
}

/** Called when the game starts or when spawned */
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraMappingContext);

	if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		LocalPlayerSubsystem->AddMappingContext(AuraMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

/** Allows the PlayerController to set up custom input bindings */
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

#pragma endregion OVERRIDES

#pragma region INPUT

/** Called for moving the player */
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetAuraAbilitySystemComponent() && GetAuraAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawControlRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawControlRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawControlRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	GetPawn()->AddMovementInput(RightDirection, InputAxisVector.X);
}

/** Called when the Shift key is pressed */
void AAuraPlayerController::ShiftPressed()
{
	bShiftKeyPressed = true;
}

/** Called when the Shift key is released */
void AAuraPlayerController::ShiftReleased()
{
	bShiftKeyPressed = false;
}

/** Callback for Input Pressed */
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetAuraAbilitySystemComponent() && GetAuraAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	bTargeting = CurrentHighlightedActor ? true : false;
	bAutoRunning = false;

	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
	}
}

/** Callback for Input Held */
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetAuraAbilitySystemComponent() && GetAuraAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (bTargeting || bShiftKeyPressed)
		{
			if (GetAuraAbilitySystemComponent())
			{
				GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
			}
		}
		else
		{
			FollowMouseCursor();
		}
	}
	else
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
	}
}

/** Callback for Input Released */
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAuraAbilitySystemComponent())
	{
		if (GetAuraAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
		{
			return;
		}
	
		GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	}
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (!bTargeting && !bShiftKeyPressed)
		{
			StartAutoRun();
		}
	}
}

#pragma endregion INPUT

#pragma region MOVEMENT

/** Generate path to the location under the mouse cursor, after a short press occured */
void AAuraPlayerController::StartAutoRun()
{
	if (FollowTime <= ShortPressThreshold)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetPawn()->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PathPoint : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
			}

			if (!NavPath->PathPoints.IsEmpty())
			{
				CachedDestination = NavPath->PathPoints.Last();
			}
			
			bAutoRunning = true;
		}

		if (GetAuraAbilitySystemComponent() && !GetAuraAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}
	}

	FollowTime = 0.f;
	bTargeting = false;
}

/** Move pawn automatically following a path to the location under the mouse cursor, after a short press occured */
void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning)
	{
		return;
	}
	
	const FVector DestinationPoint = Spline->FindLocationClosestToWorldLocation(GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector DestinationDirection = Spline->FindDirectionClosestToWorldLocation(DestinationPoint, ESplineCoordinateSpace::World);
	GetPawn()->AddMovementInput(DestinationDirection);

	const float DistanceToDestination = (DestinationPoint - CachedDestination).Length();
	if (DistanceToDestination < AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
	}
}

/** Follow mouse cursor to move pawn */
void AAuraPlayerController::FollowMouseCursor()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	if (CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
	}
	
	const FVector MovementDirection = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
	GetPawn()->AddMovementInput(MovementDirection);
}

#pragma endregion MOVEMENT

#pragma region INTERACTABLE

/** Trace hit under cursor to highlight Actors */
void AAuraPlayerController::CursorTrace(float DeltaTime)
{
	if (GetAuraAbilitySystemComponent() && GetAuraAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastHighlightedActor)
		{
			LastHighlightedActor->UnHighlightActor();
			LastHighlightedActor = nullptr;
		}

		if (CurrentHighlightedActor)
		{
			CurrentHighlightedActor->UnHighlightActor();
			CurrentHighlightedActor = nullptr;
		}
		
		return;
	}
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastHighlightedActor = CurrentHighlightedActor;
	CurrentHighlightedActor = Cast<IInteractableInterface>(CursorHit.GetActor());

	if (LastHighlightedActor != CurrentHighlightedActor)
	{
		if (LastHighlightedActor)
		{
			LastHighlightedActor->UnHighlightActor();
		}

		if (CurrentHighlightedActor)
		{
			CurrentHighlightedActor->HighlightActor();
		}
	}
}

#pragma endregion INTERACTABLE

#pragma region COMBAT

/** Show damage number above target */
void AAuraPlayerController::ClientDisplayDamage_Implementation(ACharacter* TargetCharacter, float DamageAmount, bool bInIsBlockedHit, bool bInIsCriticalHit)
{
	if (!IsValid(TargetCharacter) || !DamageTextWidgetClass || !IsLocalController())
	{
		return;
	}

	UDamageTextWidgetComponent* DamageTextWidget = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextWidgetClass);
	DamageTextWidget->RegisterComponent();
	DamageTextWidget->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageTextWidget->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageTextWidget->SetDamageText(DamageAmount, bInIsBlockedHit, bInIsCriticalHit);
}

#pragma endregion COMBAT

#pragma region GAS

/** Get Ability system component */
UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	
	return AuraAbilitySystemComponent;
}

#pragma endregion GAS

#pragma region MAGIC_CIRCLE

/** Show magic circle */
void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->SetDecalMaterial(DecalMaterial);
		}
	}

	bShowMouseCursor = false;
}

/** Hide magic circle */
void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}

	bShowMouseCursor = true;
}

/** Update spawned magic circle's location */
void AAuraPlayerController::UpdateMagicCircleLocation() const
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

#pragma endregion MAGIC_CIRCLE
