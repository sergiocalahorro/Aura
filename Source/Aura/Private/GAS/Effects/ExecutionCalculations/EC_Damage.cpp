// Copyright Sergio Calahorro

#include "GAS/Effects/ExecutionCalculations/EC_Damage.h"

// Headers - Aura
#include "Character/Data/CharacterClassInfo.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "GAS/Utils/AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
public:
	
	/** Declare Armor Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	/** Declare ArmorPenetration Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);

	/** Declare BlockChance Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	/** Declare CriticalHitChance Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);

	/** Declare CriticalHitResistance Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	/** Declare CriticalHitDamage Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	/** Declare ResistanceFire Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire);

	/** Declare ResistanceLightning Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning);

	/** Declare ResistanceArcane Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane);

	/** Declare ResistancePhysical Attribute's capture definition */
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical);

	/** Map Damage Type resistances to Attribute capture definitions */
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	/** Sets default values for this struct's properties */
	AuraDamageStatics()
	{
		// Source attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);

		// Target attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceFire, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceLightning, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceArcane, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistancePhysical, Target, false);

		// Map capture definitions to corresponding tags
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, ResistanceFireDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, ResistanceLightningDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ResistanceArcaneDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, ResistancePhysicalDef);
	}
};


static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics Statics;
	return Statics;
}

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UEC_Damage::UEC_Damage()
{
	// Armor
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);

	// Block hit
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);

	// Critical hit
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	// Resistances to Damage Types
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceFireDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightningDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceArcaneDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePhysicalDef);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called whenever the owning gameplay effect is executed */
void UEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const ICombatInterface* SourceAvatarCombat = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface* TargetAvatarCombat = Cast<ICombatInterface>(TargetAvatar);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	
	// Calculate damage value
	float FinalDamage = 0.f;
	for (auto DamageTypeToResistance : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = DamageTypeToResistance.Key;
		const FGameplayTag ResistanceDamageTypeTag = DamageTypeToResistance.Value;
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceDamageTypeTag), TEXT("EC_Damage::Execute - TagsToCaptureDefs doesn't contain Tag: [%s]"), *ResistanceDamageTypeTag.ToString());

		// ToDo: Add a DamageType GameplayTag (or GameplayTagContainer, in case an attack could use multiple Damage Types) to the EffectContext
		// That way, the DamageType from the EffectContext could be accessed and used for finding its matching Resistance in the DamageTypesToResistances map,
		// instead of unnecessarily looping through all the existing Damage Types
		
		const float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false);
		CalculateDamageByResistanceToDamageType(ResistanceDamageTypeTag, ExecutionParams, EvaluateParams, DamageTypeValue, FinalDamage);
	}

	// Check Target's BlockChance and calculate incoming Damage's value if it's a block
	const bool bIsBlockedHit = HandleBlock(ExecutionParams, EvaluateParams, FinalDamage);
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bIsBlockedHit);

	// Check Target's Armor and Source's ArmorPenetration and calculate incoming Damage's value
	HandleArmor(CharacterClassInfo, SourceAvatarCombat, TargetAvatarCombat, ExecutionParams, EvaluateParams, FinalDamage);

	// Check Target's CriticalHitChance and Source's CriticalHitResistance and calculate incoming Damage's value if it's a critical hit
	const bool bIsCriticalHit = HandleCriticalHit(CharacterClassInfo, TargetAvatarCombat, ExecutionParams, EvaluateParams, FinalDamage);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);

	// Modify IncomingDamage attribute with the calculated final Damage value
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

#pragma endregion OVERRIDES

#pragma region DAMAGE

/** Calculate final damage by checking the Resistance to the incoming Damage Type */
void UEC_Damage::CalculateDamageByResistanceToDamageType(const FGameplayTag& ResistanceDamageTypeTag, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float DamageTypeValue, float& Damage) const
{
	float ResistanceDamageTypeValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().TagsToCaptureDefs[ResistanceDamageTypeTag], EvaluateParams, ResistanceDamageTypeValue);
	ResistanceDamageTypeValue = FMath::Clamp(ResistanceDamageTypeValue, 0.f, 100.f);

	DamageTypeValue *= (100.f - ResistanceDamageTypeValue) / 100.f;
	Damage += DamageTypeValue;
}

/** Check Target's BlockChance in order to calculate Damage taken if there's a successful block */
bool UEC_Damage::HandleBlock(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const
{
	// Capture BlockChance on Target in order to determine if there was a successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	// Halve Damage's value if it's a block
	const bool bBlocked = FMath::RandRange(1.f, 100.f) < TargetBlockChance;
	Damage = bBlocked ? Damage / 2.f : Damage;

	return bBlocked;
}

/** Check Target's Armor and Source's ArmorPenetration in order to calculate Damage taken */
void UEC_Damage::HandleArmor(const UCharacterClassInfo* CharacterClassInfo, const ICombatInterface* SourceAvatarCombat, const ICombatInterface* TargetAvatarCombat, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const
{
	// Capture Armor on Target
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParams, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// Capture ArmorPenetration on Source, which ignores a percentage of the Target's Armor
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	// Retrieve the armor penetration's coefficient based on the Source's level
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceAvatarCombat->GetCurrentLevel());

	// Retrieve the effective armor's coefficient based on the Target's level
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetAvatarCombat->GetCurrentLevel());

	// Apply Source's armor penetration and its coefficient to the Target's armor, in order to calculate the effective armor
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	// Calculate final Damage that will be dealt based on the effective armor and its coefficient
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
}

/** Check Target's CriticalHitChance and Source's CriticalHitResistance in order to calculate Damage taken */
bool UEC_Damage::HandleCriticalHit(const UCharacterClassInfo* CharacterClassInfo, const ICombatInterface* TargetAvatarCombat, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& Damage) const
{
	// Capture CriticalHitChance on Source in order to determine if there was a critical hit
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParams, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	
	// Capture CriticalHitResistance on Target in order to reduce the chance of a critical hit
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParams, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	// Capture CriticalHitDamage on Source in order to determine the damage bonus
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParams, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	// Retrieve the critical hit resistance's coefficient based on the Target's level
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetAvatarCombat->GetCurrentLevel());

	// Apply Target's critical hit resistance and its coefficient in order to reduce the critical hit chance
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

	// Double Damage's value and add to its value the Source's CriticalHitDamage if it's a critical hit
	const bool bCriticalHit = FMath::RandRange(1.f, 100.f) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? (Damage * 2.f) + SourceCriticalHitDamage : Damage;

	return bCriticalHit;
}

#pragma endregion DAMAGE