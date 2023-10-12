// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "EC_Damage.generated.h"

class ICombatInterface;
// Forward declarations - Aura
class UCharacterClassInfo;

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

	/** Check Target's BlockChance in order to calculate Damage taken if there's a successful block */
	void HandleBlock(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const;

	/** Check Target's Armor and Source's ArmorPenetration in order to calculate Damage taken */
	void HandleArmor(const UCharacterClassInfo* CharacterClassInfo, const ICombatInterface* SourceAvatarCombat, const ICombatInterface* TargetAvatarCombat, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const;

	/** Check Target's CriticalHitChance and Source's CriticalHitResistance in order to calculate Damage taken */
	void HandleCriticalHit(const UCharacterClassInfo* CharacterClassInfo, const ICombatInterface* TargetAvatarCombat, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const;

#pragma endregion DAMAGE
	
};
