// Copyright Sergio Calahorro

#include "Player/AuraPlayerController.h"

// Headers - Unreal Engine
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/InteractableInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation() */
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace(DeltaTime);
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
	AuraInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

#pragma endregion OVERRIDES

#pragma region INPUT

/** Move player */
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

/** Callback for Input Pressed */
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

/** Callback for Input Released */
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetAuraAbilitySystemComponent())
	{
		return;
	}
	
	GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

/** Callback for Input Held */
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetAuraAbilitySystemComponent())
	{
		return;
	}
	
	GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

#pragma endregion INPUT

#pragma region INTERACTABLE

/** Trace hit under cursor to highlight Actors */
void AAuraPlayerController::CursorTrace(float DeltaTime)
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastHighlightedActor = CurrentHighlightedActor;
	CurrentHighlightedActor = Cast<IInteractableInterface>(CursorHit.GetActor());

	if (LastHighlightedActor)
	{
		if (CurrentHighlightedActor)
		{
			if (LastHighlightedActor != CurrentHighlightedActor)
			{
				// As the last highlighted Actor is different to the current one, un-highlight it and highlight the new one 
				LastHighlightedActor->UnHighlightActor();
				CurrentHighlightedActor->HighlightActor();
			}
		}
		else
		{
			// As there isn't a highlighted Actor, un-highlight the last one 
			LastHighlightedActor->UnHighlightActor();
		}
	}
	else
	{
		if (CurrentHighlightedActor)
		{
			// As there isn't a highlighted Actor yet, highlight the current one 
			CurrentHighlightedActor->HighlightActor();
		}
	}
}

#pragma endregion INTERACTABLE

#pragma region GAS

/** Get Ability system component */
UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	
	return AuraAbilitySystemComponent;
}

#pragma endregion GAS