﻿// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraRangedAttack.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "Actor/Projectile/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraRangedAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const AActor* CombatTarget = ICombatInterface::Execute_GetCombatTarget(ActorInfo->AvatarActor.Get()))
	{
		if (ICombatInterface* AttackingActor = Cast<ICombatInterface>(ActorInfo->AvatarActor.Get()))
		{
			AttackingActor->SetFacingTarget(CombatTarget->GetActorLocation());
			ProjectileTargetLocation = CombatTarget->GetActorLocation();
		}
	}
		
	const TArray<FAttackData> Attacks = ICombatInterface::Execute_GetAllAttacks(ActorInfo->AvatarActor.Get());
	if (Attacks.IsEmpty() && !SpawnProjectileEventTag.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	CurrentAttackData = GetAttackToUse(SpawnProjectileEventTag, Attacks);

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentAttackData.AttackMontage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraRangedAttack::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraRangedAttack::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraRangedAttack::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CurrentAttackData.AttackMontageTag);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UAuraRangedAttack::SpawnProjectile);
	WaitEventTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraRangedAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

#pragma region RANGED_ATTACK

/** Perform Melee Attack */
void UAuraRangedAttack::SpawnProjectile(FGameplayEventData Payload)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	check(CurrentAttackData.ProjectileClass);
	
	FTransform SpawnTransform;
	const ICombatInterface* AttackingActor = CastChecked<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector SpawnLocation = AttackingActor->GetCombatSocketLocation(CurrentAttackData.CombatSocketTag);
	const FRotator SpawnRotation = (ProjectileTargetLocation - SpawnLocation).Rotation();
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(CurrentAttackData.ProjectileClass,  SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const int32 Level = GetAbilityLevel();
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, Level, EffectContextHandle);

	for (auto DamageType : DamageTypes)
	{
		const float ScaledDamage = DamageType.Value.GetValueAtLevel(Level);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = EffectSpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}

#pragma endregion RANGED_ATTACK
