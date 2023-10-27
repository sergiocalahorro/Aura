// Copyright Sergio Calahorro

#include "GAS/Effects/ModMagnitudeCalculations/MMC_MaxMana.h"

// Headers - Aura
#include "GAS/Attributes/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Calculate the base magnitude of the gameplay effect modifier, given the specified spec */
float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Get Intelligence value
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluateParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	// Get Source's current level
	UObject* SourceObject = Spec.GetContext().GetSourceObject();
	const int32 Level = SourceObject->Implements<UCombatInterface>() ? ICombatInterface::Execute_GetCurrentLevel(SourceObject) : 1;

	constexpr float BaseMaxMana = 50.f;
	constexpr float IntelligenceCoefficient = 2.5f;
	constexpr float LevelCoefficient = 15.f;
	return BaseMaxMana + (Intelligence * IntelligenceCoefficient) + (Level * LevelCoefficient);
}

#pragma endregion OVERRIDES