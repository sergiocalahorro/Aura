// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

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

	/** Calculate final damage by checking the Resistance to the incoming Damage Type */
	void CalculateDamageByResistanceToDamageType(const FGameplayTag& ResistanceDamageTypeTag, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float DamageTypeValue, float& Damage) const;

	/** Check Target's BlockChance in order to calculate Damage taken if there's a successful block */
	bool HandleBlock(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const;

	/** Check Target's Armor and Source's ArmorPenetration in order to calculate Damage taken */
	void HandleArmor(const UCharacterClassInfo* CharacterClassInfo, const ICombatInterface* SourceAvatarCombat, const ICombatInterface* TargetAvatarCombat, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const;

	/** Check Target's CriticalHitChance and Source's CriticalHitResistance in order to calculate Damage taken */
	bool HandleCriticalHit(const UCharacterClassInfo* CharacterClassInfo, const ICombatInterface* TargetAvatarCombat, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const;

#pragma endregion DAMAGE
	
};
