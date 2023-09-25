// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"

#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UMMC_MaxMana();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Calculate the base magnitude of the gameplay effect modifier, given the specified spec */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

#pragma endregion OVERRIDES

#pragma region ATTRIBUTES

private:

	/** Intelligence's Attribute definition */
	FGameplayEffectAttributeCaptureDefinition IntelligenceDefinition;

#pragma endregion ATTRIBUTES
	
};
