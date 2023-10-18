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

	TArray<FTaggedMontage> TaggedAttackMontages = ICombatInterface::Execute_GetAttackMontages(ActorInfo->AvatarActor.Get());
	if (TaggedAttackMontages.IsEmpty())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	TaggedAttackMontage = TaggedAttackMontages[FMath::RandRange(0, TaggedAttackMontages.Num() - 1)];

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TaggedAttackMontage.Montage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraMeleeAttack::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TaggedAttackMontage.MontageTag);
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
		const FVector CombatSocketLocation = CombatAvatarActor->GetCombatSocketLocation(TaggedAttackMontage.SocketTag);
		TArray<AActor*> ActorsToDamage;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(AvatarActor);
		UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(this, CombatSocketLocation, AttackRadius, ActorsToIgnore, ActorsToDamage);

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
			GameplayCueParams.AggregatedSourceTags = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(TaggedAttackMontage.MontageTag);
			K2_ExecuteGameplayCueWithParams(FAuraGameplayTags::Get().GameplayCue_MeleeImpact, GameplayCueParams);
		}
	}
}

#pragma endregion MELEE_ATTACK

