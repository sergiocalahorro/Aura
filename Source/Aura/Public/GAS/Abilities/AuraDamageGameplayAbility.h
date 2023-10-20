// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraGameplayAbility.h"
#include "Interaction/AttackData.h"

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

protected:

	/** Damage's effect class for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Damage values per type */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

#pragma endregion DAMAGE

#pragma region ATTACK

protected:
	
	/** Get attack to use */
	virtual FAttackData GetAttackToUse(const TArray<FAttackData>& Attacks, const FGameplayTag& AttackTag = FGameplayTag());

protected:
	
	/** Current attack data to use in this ability */
	FAttackData CurrentAttackData;

	/** Type of attack supported by this ability */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Attack")
	EAttackType AttackType;

#pragma endregion ATTACK
	
};
