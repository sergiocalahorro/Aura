// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraMeleeAttack.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const AActor* CombatTarget = ICombatInterface::Execute_GetCombatTarget(ActorInfo->AvatarActor.Get()))
	{
		if (ICombatInterface* AttackingActor = Cast<ICombatInterface>(ActorInfo->AvatarActor.Get()))
		{
			AttackingActor->SetFacingTarget(CombatTarget->GetActorLocation());
		}
	}
	
	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MeleeAttackMontage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FAuraGameplayTags::Get().Event_Montage_Attack_Melee);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UAuraMeleeAttack::MeleeAttack);
	WaitEventTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(PlayMontageTask))
	{
		PlayMontageTask->EndTask();
	}

	if (IsValid(WaitEventTask))
	{
		WaitEventTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region MELEE_ATTACK

/** Perform Melee Attack */
void UAuraMeleeAttack::MeleeAttack(FGameplayEventData Payload)
{
	if (const ICombatInterface* AttackingActor = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		DrawDebugSphere(GetWorld(), AttackingActor->GetCombatSocketLocation(), 45.f, 12, FColor::Red, false, 0.5f);
	}
}

#pragma endregion MELEE_ATTACK

