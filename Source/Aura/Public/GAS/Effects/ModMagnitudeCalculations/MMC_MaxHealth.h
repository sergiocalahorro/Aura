// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"

#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UMMC_MaxHealth();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Calculate the base magnitude of the gameplay effect modifier, given the specified spec */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

#pragma endregion OVERRIDES

#pragma region ATTRIBUTES

private:

	/** Vigor's Attribute definition */
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;

#pragma endregion ATTRIBUTES
	
};
