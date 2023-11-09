// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayTags/AuraGameplayTags.h"

#include "EC_Damage.generated.h"

struct FAuraGameplayEffectContext;
// Forward declarations - Aura
class UCharacterClassInfo;
class ICombatInterface;

/**
 * 
 */
UCLASS()
class AURA_API UEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UEC_Damage();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called whenever the owning gameplay effect is executed */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

#pragma endregion OVERRIDES

#pragma region DAMAGE

private:

	/** Calculate damage taking into account the resistances to the damage types */
	float CalculateDamage(const FGameplayTag& DamageTypeTag, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& EffectSpec, const FAggregatorEvaluateParameters& EvaluateParams, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const;

	/** Determine whether a debuff occured */
	void HandleDebuff(const FGameplayTag& DamageTypeTag, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& EffectSpec, const FAggregatorEvaluateParameters& EvaluateParams, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const;
	
	/** Check Target's BlockChance in order to calculate Damage taken if there's a successful block */
	bool HandleBlock(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& OutDamage) const;

	/** Check Target's Armor and Source's ArmorPenetration in order to calculate Damage taken */
	void HandleArmor(const UCharacterClassInfo* CharacterClassInfo, int32 SourceLevel, int32 TargetLevel, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& OutDamage) const;

	/** Check Target's CriticalHitChance and Source's CriticalHitResistance in order to calculate Damage taken */
	bool HandleCriticalHit(const UCharacterClassInfo* CharacterClassInfo, int32 TargetLevel, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& OutDamage) const;

#pragma endregion DAMAGE
	
};
