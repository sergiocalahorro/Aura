// Copyright Sergio Calahorro

#include "GAS/Effects/ModMagnitudeCalculations/MMC_MaxHealth.h"

// Headers - Aura
#include "GAS/Attributes/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDefinition);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Calculate the base magnitude of the gameplay effect modifier, given the specified spec */
float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Get Vigor value
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluateParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// Get Source's current level
	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level = CombatInterface->GetCurrentLevel();

	constexpr float BaseMaxHealth = 80.f;
	constexpr float VigorCoefficient = 2.5f;
	constexpr float LevelCoefficient = 10.f;
	return BaseMaxHealth + (Vigor * VigorCoefficient) + (Level * LevelCoefficient);
}

#pragma endregion OVERRIDES