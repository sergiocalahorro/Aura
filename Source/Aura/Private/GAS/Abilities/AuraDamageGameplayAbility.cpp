// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraDamageGameplayAbility.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#pragma region DAMAGE

/** Apply damage to target Actor */
void UAuraDamageGameplayAbility::ApplyDamage(AActor* TargetActor)
{
	const int32 Level = GetAbilityLevel();
	const FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, Level);

	for (auto DamageType : DamageTypes)
	{
		const float ScaledDamage = DamageType.Value.GetValueAtLevel(Level);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType.Key, ScaledDamage);
	}

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
	}
}

/** Get damage value by damage type tag */
float UAuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageTypeTag) const
{
	checkf(DamageTypes.Contains(DamageTypeTag), TEXT("GameplayAbility [%s] does not contain DamageTypeTag [%s]"), *GetNameSafe(this), *DamageTypeTag.ToString());
	return DamageTypes[DamageTypeTag].GetValueAtLevel(InLevel);
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
