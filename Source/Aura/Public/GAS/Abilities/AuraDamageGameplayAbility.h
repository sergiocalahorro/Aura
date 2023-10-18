// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraGameplayAbility.h"
#include "Interaction/TaggedMontage.h"

#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

#pragma region DAMAGE

protected:

	/** Apply damage to target Actor */
	void ApplyDamage(AActor* TargetActor);

	/** Get attack to use (montage and tag) */
	virtual FTaggedMontage GetTaggedAttackMontageToUse(const FGameplayTag& AttackTag, const TArray<FTaggedMontage>& TaggedAttackMontages);

protected:

	/** Damage's effect class for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Damage values per type */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

#pragma endregion DAMAGE
};
