// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraGameplayAbility.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraGameplayAbility::UAuraGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#pragma endregion INITIALIZATION

#pragma region ATTACK

/** Get attack to use */
FAttackData UAuraGameplayAbility::GetAttackToUse(const TArray<FAttackData>& Attacks, const FGameplayTag& AttackTag)
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
