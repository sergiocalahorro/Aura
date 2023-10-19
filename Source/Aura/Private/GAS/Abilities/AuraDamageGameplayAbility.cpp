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

/** Get attack to use */
FAttackData UAuraDamageGameplayAbility::GetAttackToUse(const FGameplayTag& AttackTag, const TArray<FAttackData>& Attacks)
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

#pragma endregion DAMAGE