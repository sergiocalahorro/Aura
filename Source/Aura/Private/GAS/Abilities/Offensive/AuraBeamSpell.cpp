// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/AuraBeamSpell.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayCueFunctionLibrary.h"

// Headers - Aura
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura.h"
#include "GAS/AbilityTasks/AbilityTask_TargetDataUnderMouse.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraBeamSpell::UAuraBeamSpell()
{
	AttackType = EAttackType::Ranged;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraBeamSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	TargetDataUnderMouseTask = UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(this);
	TargetDataUnderMouseTask->ReceivedTargetData.AddUniqueDynamic(this, &UAuraBeamSpell::TargetDataReceived);
	TargetDataUnderMouseTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraBeamSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(TargetDataUnderMouseTask))
	{
		TargetDataUnderMouseTask->EndTask();
	}

	if (IsValid(WaitInputReleaseTask))
	{
		WaitInputReleaseTask->EndTask();
	}

	UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(FirstTarget, CueBeamLoop, FirstTargetCueParams);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region BEAM

/** Functionality performed once target data under mouse is received */
void UAuraBeamSpell::TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (TargetDataHandle.Num() == 0)
	{
		return;
	}
	
	if (ICombatInterface* AttackingActor = CastChecked<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FHitResult MouseHit = *TargetDataHandle.Get(0)->GetHitResult();
		if (!MouseHit.bBlockingHit)
		{
			CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
			return;
		}
		
		MouseHitLocation = MouseHit.Location;
		MouseHitActor = MouseHit.GetActor();
		AttackingActor->SetFacingTarget(MouseHitLocation);

		if (CurrentActorInfo)
		{
			OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
			OwnerCharacterMovementComponent = Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent.Get());
		}
		
		if (OwnerPlayerController)
		{
			OwnerPlayerController->bShowMouseCursor = false;
		}

		K2_ExecuteGameplayCue(CueBeamStart, GetContextFromOwner(TargetDataHandle));
		
		PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentAttackData.AttackMontage);
		PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraBeamSpell::K2_EndAbility);
		PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraBeamSpell::K2_EndAbility);
		PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraBeamSpell::K2_EndAbility);
		PlayMontageTask->ReadyForActivation();
		
		WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CurrentAttackData.AttackMontageTag, nullptr, true, true);
		WaitEventTask->EventReceived.AddUniqueDynamic(this, &UAuraBeamSpell::EventReceivedSpawnBeam);
		WaitEventTask->ReadyForActivation();
		
		WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
		WaitInputReleaseTask->OnRelease.AddUniqueDynamic(this, &UAuraBeamSpell::InputReleased);
		WaitInputReleaseTask->ReadyForActivation();
	}
}

/** Event received for spawning beam */
void UAuraBeamSpell::EventReceivedSpawnBeam(FGameplayEventData Payload)
{
	if (OwnerCharacterMovementComponent)
	{
		OwnerCharacterMovementComponent->DisableMovement();
	}

	SpawnBeam();
}

/** Spawn beam */
void UAuraBeamSpell::SpawnBeam()
{
	TraceFirstTarget(MouseHitLocation);
	
	FirstTargetCueParams.Location = MouseHitLocation;
	FirstTargetCueParams.SourceObject = MouseHitActor;
	FirstTargetCueParams.TargetAttachComponent = GetAvatarActorFromActorInfo()->Implements<UCombatInterface>() ? ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo()) : nullptr;

	FirstTarget = MouseHitActor->Implements<UCombatInterface>() ? MouseHitActor : GetAvatarActorFromActorInfo();
	UGameplayCueFunctionLibrary::AddGameplayCueOnActor(FirstTarget, CueBeamLoop, FirstTargetCueParams);
}

/** Functionality performed once the input is released */
void UAuraBeamSpell::InputReleased(float TimeHeld)
{
	if (OwnerPlayerController)
	{
		OwnerPlayerController->bShowMouseCursor = true;
	}

	if (OwnerCharacterMovementComponent)
	{
		OwnerCharacterMovementComponent->SetMovementMode(MOVE_Walking);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

/** Perform trace to first target */
void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor->Implements<UCombatInterface>())
	{
		return;
	}
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(AvatarActor);
	const FVector WeaponSocketLocation = ICombatInterface::Execute_GetWeaponSocketLocation(AvatarActor, BeamStartSocket);

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(
		GetAvatarActorFromActorInfo(),
		WeaponSocketLocation,
		BeamTargetLocation,
		BeamTraceRadius,
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
}

#pragma endregion BEAM