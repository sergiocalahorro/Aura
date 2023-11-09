// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraDamageGameplayAbility.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/Utils/AuraAbilityTypes.h"

#pragma region DAMAGE

/** Apply damage to target Actor */
void UAuraDamageGameplayAbility::ApplyDamage(AActor* TargetActor) const
{
	const FDamageEffectParams DamageEffectParams = MakeDamageEffectParams(TargetActor);
	UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
}

/** Make damage effect's params from class defaults */
FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParams(AActor* TargetActor) const
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
