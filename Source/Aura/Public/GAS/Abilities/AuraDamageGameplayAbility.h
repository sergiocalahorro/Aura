// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraGameplayAbility.h"
#include "Interaction/AttackData.h"

#include "AuraDamageGameplayAbility.generated.h"

// Forward declarations - Aura
struct FDamageEffectParams;

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
	void ApplyDamage(AActor* TargetActor) const;

	/** Make damage effect's params from class defaults */
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor = nullptr) const;
	
#pragma endregion DAMAGE

protected:

	/** Damage's effect class for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Damage type tag */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	FGameplayTag DamageType;

	/** Damage value */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage")
	FScalableFloat Damage;

	/** Chance to apply debuff */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Debuff", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 100.f, UIMax = 100.f, Delta = 1.f, Units = "Percent"))
	float DebuffChance = 20.f;

	/** Damage caused when debuff is applied */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Debuff", meta = (ClampMin = 0.f, UIMin = 0.f))
	float DebuffDamage = 5.f;

	/** Duration for the applied debuff */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Debuff", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Seconds"))
	float DebuffDuration = 5.f;

	/** Frequency for the applied debuff */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Debuff", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Seconds"))
	float DebuffFrequency = 1.f;

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
