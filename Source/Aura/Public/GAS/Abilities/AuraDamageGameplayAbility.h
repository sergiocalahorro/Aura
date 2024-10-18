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

#pragma region OVERRIDES
	
protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

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

	/** Attack Montage's tag */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Attack")
	FGameplayTag AttackMontageTag;

#pragma endregion ATTACK

#pragma region DAMAGE

protected:

	/** Apply damage to target Actor */
	void ApplyDamage(AActor* TargetActor, bool bApplyDebuff = true) const;

	/** Make damage effect's params from class defaults */
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor = nullptr) const;

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

	/** Chance to cause a knockback when applying normal damage */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Knockback", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 100.f, UIMax = 100.f, Delta = 1.f, Units = "Percent"))
	float KnockbackChance = 0.f;

	/** Force applied to knockback on normal damage */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Knockback", meta = (ClampMin = 0.f, UIMin = 0.f))
	float KnockbackForceMagnitude = 500.f;
	
	/** Impulse applied on fatal damage */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Death", meta = (ClampMin = 0.f, UIMin = 0.f))
	float DeathImpulseMagnitude = 15000.f;

	/** Whether damage to be applied is radial */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Radial")
	bool bRadialDamage = false;

	/** Radial damage's inner radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Radial", meta = (EditConditionHides, EditCondition = "bRadialDamage", ClampMin = 0.f, UIMin = 0.f))
	float RadialDamageInnerRadius = 0.f;

	/** Radial damage's outer radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage|Radial", meta = (EditConditionHides, EditCondition = "bRadialDamage", ClampMin = 0.f, UIMin = 0.f))
	float RadialDamageOuterRadius = 0.f;

	/** Radial damage's origin */
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;

#pragma endregion DAMAGE
	
};
