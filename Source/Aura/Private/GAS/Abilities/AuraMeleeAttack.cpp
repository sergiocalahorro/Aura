// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraMeleeAttack.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// Headers - Aura
#include "BlueprintGameplayTagLibrary.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
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

	const TArray<FAttackData> Attacks = ICombatInterface::Execute_GetAllAttacks(ActorInfo->AvatarActor.Get());
	if (Attacks.IsEmpty() && !DamageEventTag.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	CurrentAttackData = GetAttackToUse(DamageEventTag, Attacks);

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentAttackData.AttackMontage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CurrentAttackData.AttackMontageTag);
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
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (const ICombatInterface* CombatAvatarActor = Cast<ICombatInterface>(AvatarActor))
	{
		const FVector CombatSocketLocation = CombatAvatarActor->GetCombatSocketLocation(CurrentAttackData.CombatSocketTag);
		TArray<AActor*> ActorsToDamage;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(AvatarActor);
		UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(this, CombatSocketLocation, CurrentAttackData.MeleeAttackRadius, ActorsToIgnore, ActorsToDamage);

		bool bHasHitTarget = false;
		for (AActor* DamagedActor : ActorsToDamage)
		{
			if (!UAuraAbilitySystemLibrary::AreActorsFriends(AvatarActor, DamagedActor))
			{
				bHasHitTarget = true;
				ApplyDamage(DamagedActor);
			}
		}

		if (bHasHitTarget)
		{
			FGameplayCueParameters GameplayCueParams;
			GameplayCueParams.Location = CombatSocketLocation;
			GameplayCueParams.EffectCauser = AvatarActor;
			GameplayCueParams.SourceObject = ICombatInterface::Execute_GetCombatTarget(AvatarActor);
			GameplayCueParams.AggregatedSourceTags = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(CurrentAttackData.AttackMontageTag);
			K2_ExecuteGameplayCueWithParams(FAuraGameplayTags::Get().GameplayCue_MeleeImpact, GameplayCueParams);
		}

		DrawDebugSphere(GetWorld(), CombatSocketLocation, CurrentAttackData.MeleeAttackRadius, 12, bHasHitTarget ? FColor::Green : FColor::Red, false, 0.5f);
	}
}

#pragma endregion MELEE_ATTACK

