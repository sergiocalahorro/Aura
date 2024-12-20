﻿// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraDamageGameplayAbility.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/Utils/AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraDamageGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const TArray<FAttackData> Attacks = ICombatInterface::Execute_GetAttacksOfType(ActorInfo->AvatarActor.Get(), AttackType);
	if (Attacks.IsEmpty() && !AttackMontageTag.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CurrentAttackData = GetAttackToUse(Attacks, AttackMontageTag);
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraDamageGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region DAMAGE

/** Apply damage to target Actor */
void UAuraDamageGameplayAbility::ApplyDamage(AActor* TargetActor, bool bApplyDebuff, const FVector& RadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverrideDeathImpulseDirection, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	if (!HasAuthority(&CurrentActivationInfo))
	{
		return;
	}
	
	if (IsValid(TargetActor) && TargetActor != GetAvatarActorFromActorInfo())
	{
		const FDamageEffectParams DamageEffectParams = MakeDamageEffectParams(TargetActor, RadialDamageOrigin, bOverrideKnockbackDirection, KnockbackDirectionOverride, bOverrideDeathImpulseDirection, DeathImpulseDirectionOverride, bOverridePitch, PitchOverride);
		UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams, bApplyDebuff);
	}
}

/** Make damage effect's params from class defaults */
FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParams(AActor* TargetActor, const FVector& RadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverrideDeathImpulseDirection, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageEffectClass = DamageEffectClass;
	DamageEffectParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParams.KnockbackForceMagnitude = KnockbackForceMagnitude;
	DamageEffectParams.KnockbackChance = KnockbackChance;
	DamageEffectParams.bRadialDamage = bRadialDamage;
	DamageEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
	DamageEffectParams.RadialDamageOrigin = RadialDamageOrigin;

	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = bOverridePitch ? PitchOverride : 45.f;
		const FVector DirectionToTarget = Rotation.Vector();

		if (!bOverrideKnockbackDirection)
		{
			DamageEffectParams.KnockbackForce = DirectionToTarget * KnockbackForceMagnitude;
		}

		if (!bOverrideDeathImpulseDirection)
		{
			DamageEffectParams.DeathImpulse = DirectionToTarget * DeathImpulseMagnitude;
		}
	}
	
	// Knockback
	if (FMath::RandRange(1.f, 100.f) < KnockbackChance)
	{
		if (bOverrideKnockbackDirection)
		{
			KnockbackDirectionOverride.Normalize();
			DamageEffectParams.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude;
	
			if (bOverridePitch)
			{
				FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
				KnockbackRotation.Pitch = PitchOverride;
				DamageEffectParams.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
			}
		}
	}

	// Death impulse
	if (bOverrideDeathImpulseDirection)
	{
		DeathImpulseDirectionOverride.Normalize();
		DamageEffectParams.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			DamageEffectParams.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}

	return DamageEffectParams;
}

#pragma endregion DAMAGE

#pragma region ATTACK

/** Get attack to use */
FAttackData UAuraDamageGameplayAbility::GetAttackToUse(const TArray<FAttackData>& Attacks, const FGameplayTag& AttackTag)
{
	if (AttackTag.IsValid())
	{
		for (const FAttackData& AttackData : Attacks)
		{
			if (AttackTag.MatchesTagExact(AttackData.AttackMontageTag))
			{
				// In case AttackTag is valid, choose the TaggedAttackMontage associated with its AttackMontageTag
				return AttackData;
			}
		}
	}

	// In case AttackTag isn't valid, pick a random TaggedAttackMontage
	return Attacks[FMath::RandRange(0, Attacks.Num() - 1)];
}

#pragma endregion ATTACK
