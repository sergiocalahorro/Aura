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
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
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
	DestroyBeams();
	GetWorld()->GetTimerManager().ClearTimer(DamageCostTimerHandle);
	
	if (IsValid(TargetDataUnderMouseTask))
	{
		TargetDataUnderMouseTask->EndTask();
	}

	if (IsValid(WaitInputReleaseTask))
	{
		WaitInputReleaseTask->EndTask();
	}
	
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

	GetWorld()->GetTimerManager().SetTimer(DamageCostTimerHandle, this, &UAuraBeamSpell::ApplyDamageAndCost, DamageCostDeltaTime, true);
}

/** Spawn beam */
void UAuraBeamSpell::SpawnBeam()
{
	TraceFirstTarget(MouseHitLocation);
	FirstTarget = MouseHitActor->Implements<UCombatInterface>() ? MouseHitActor : GetAvatarActorFromActorInfo();

	FirstTargetCueParams.Location = MouseHitLocation;
	FirstTargetCueParams.SourceObject = MouseHitActor;
	FirstTargetCueParams.TargetAttachComponent = GetAvatarActorFromActorInfo()->Implements<UCombatInterface>() ? ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo()) : nullptr;
	UGameplayCueFunctionLibrary::AddGameplayCueOnActor(FirstTarget, CueBeamLoop, FirstTargetCueParams);
	
	const int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel(), MaxPropagationTargets);
	PropagateBeamsToAdditionalTargets(MouseHitActor, NumAdditionalTargets);
}

/** Spawn beams and propagate them to additional targets */
void UAuraBeamSpell::PropagateBeamsToAdditionalTargets(AActor* InitialTarget, int32 NumAdditionalTargets)
{
	if (!InitialTarget->Implements<UCombatInterface>())
	{
		return;
	}

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(InitialTarget))
	{
		CombatInterface->GetDeathDelegate().AddUniqueDynamic(this, &UAuraBeamSpell::OnTargetDestroyed);
	}
	
	TArray<AActor*> AdditionalTargets;
	GetTargetsInPropagationRadius(NumAdditionalTargets, AdditionalTargets);
	for (AActor* Target : AdditionalTargets)
	{
		FGameplayCueParameters AdditionalTargetCueParams;
		AdditionalTargetCueParams.Location = Target->GetActorLocation();
		AdditionalTargetCueParams.SourceObject = Target;
		AdditionalTargetCueParams.TargetAttachComponent = InitialTarget->GetRootComponent();
		AdditionalTargetsToCueParams.Add(Target, AdditionalTargetCueParams);
		UGameplayCueFunctionLibrary::AddGameplayCueOnActor(Target, CueBeamLoop, AdditionalTargetCueParams);
		
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			CombatInterface->GetDeathDelegate().AddUniqueDynamic(this, &UAuraBeamSpell::OnTargetDestroyed);
		}
	}
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
		true);

	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
}

/** Get additional targets inside propagation radius */
void UAuraBeamSpell::GetTargetsInPropagationRadius(int32 NumAdditionalTargets, TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetAvatarActorFromActorInfo());
	IgnoredActors.Add(MouseHitActor);

	TArray<AActor*> TargetsInPropagationRadius;
	UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
		GetAvatarActorFromActorInfo(),
		MouseHitActor->GetActorLocation(),
		BeamPropagationRadius,
		IgnoredActors,
		TargetsInPropagationRadius);
	
	UAuraAbilitySystemLibrary::GetClosestActors(NumAdditionalTargets, MouseHitActor->GetActorLocation(), TargetsInPropagationRadius, OutAdditionalTargets);
}

/** Functionality performed when a target is destroyed */
void UAuraBeamSpell::OnTargetDestroyed(AActor* ActorDestroyed)
{
	const FGameplayCueParameters BeamLoopCueParams = (ActorDestroyed == FirstTarget) ? FirstTargetCueParams : AdditionalTargetsToCueParams[ActorDestroyed];
	UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(ActorDestroyed, CueBeamLoop, BeamLoopCueParams);
}

/** Destroy beams */
void UAuraBeamSpell::DestroyBeams()
{
	UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(FirstTarget, CueBeamLoop, FirstTargetCueParams);

	if (FirstTarget->Implements<UCombatInterface>())
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(FirstTarget))
		{
			CombatInterface->GetDeathDelegate().RemoveDynamic(this, &UAuraBeamSpell::OnTargetDestroyed);
		}
		
		for (auto& Pair : AdditionalTargetsToCueParams)
		{
			UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(Pair.Key, CueBeamLoop, Pair.Value);

			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Pair.Key))
			{
				CombatInterface->GetDeathDelegate().RemoveDynamic(this, &UAuraBeamSpell::OnTargetDestroyed);
			}
		}

		AdditionalTargetsToCueParams.Empty();
	}
}

#pragma endregion BEAM

#pragma region DAMAGE

/** Apply damage and cost */
void UAuraBeamSpell::ApplyDamageAndCost()
{
	if (CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		if (!MouseHitActor->Implements<UCombatInterface>())
		{
			return;
		}

		if (ICombatInterface::Execute_IsDead(MouseHitActor))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		else
		{
			ApplyDamage(MouseHitActor, false);
			for (const auto& Pair : AdditionalTargetsToCueParams)
			{
				ApplyDamage(Pair.Key, false);
			}
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

#pragma endregion DAMAGE