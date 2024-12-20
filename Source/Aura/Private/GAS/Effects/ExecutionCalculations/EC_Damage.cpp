// Copyright Sergio Calahorro

#include "GAS/Effects/ExecutionCalculations/EC_Damage.h"

// Headers - Aura
#include "Character/Data/CharacterClassInfo.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "GAS/Utils/AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

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
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	// Map capture definitions to corresponding tags
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().ResistanceFireDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().ResistanceLightningDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ResistanceArcaneDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().ResistancePhysicalDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const int32 SourceLevel = SourceAvatar->Implements<UCombatInterface>() ? ICombatInterface::Execute_GetCurrentLevel(SourceAvatar) : 1;
	const int32 TargetLevel = SourceAvatar->Implements<UCombatInterface>() ? ICombatInterface::Execute_GetCurrentLevel(TargetAvatar) : 1;

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();

	FGameplayTag DamageTypeTag = UAuraAbilitySystemLibrary::GetDamageType(EffectContextHandle);

	// Determine whether a debuff occured
	HandleDebuff(DamageTypeTag, ExecutionParams, EffectSpec, EvaluateParams, TagsToCaptureDefs);

	// Calculate damage value
	float Damage = CalculateDamage(DamageTypeTag, ExecutionParams, EffectSpec, EvaluateParams, TagsToCaptureDefs, TargetAvatar, SourceAvatar);

	// Check Target's BlockChance and calculate incoming Damage's value if it's a block
	const bool bIsBlockedHit = HandleBlock(ExecutionParams, EvaluateParams, Damage);
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bIsBlockedHit);

	// Check Target's Armor and Source's ArmorPenetration and calculate incoming Damage's value
	HandleArmor(CharacterClassInfo, SourceLevel, TargetLevel, ExecutionParams, EvaluateParams, Damage);

	// Check Target's CriticalHitChance and Source's CriticalHitResistance and calculate incoming Damage's value if it's a critical hit
	const bool bIsCriticalHit = HandleCriticalHit(CharacterClassInfo, TargetLevel, ExecutionParams, EvaluateParams, Damage);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);

	// Modify IncomingDamage attribute with the calculated final Damage value
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

#pragma endregion OVERRIDES

#pragma region DAMAGE

/** Calculate damage taking into account the resistances to the damage types */
float UEC_Damage::CalculateDamage(const FGameplayTag& DamageTypeTag, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& EffectSpec, const FAggregatorEvaluateParameters& EvaluateParams, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs, AActor* TargetAvatar, AActor* SourceAvatar) const
{
	const FGameplayTag ResistanceDamageTypeTag = *FAuraGameplayTags::Get().DamageTypesToResistances.Find(DamageTypeTag);

	checkf(TagsToCaptureDefs.Contains(ResistanceDamageTypeTag), TEXT("EC_Damage::CalculateDamage - TagsToCaptureDefs doesn't contain Tag: [%s]"), *ResistanceDamageTypeTag.ToString());

	// Get base damage associated to the damage type
	float Damage = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false);

	if (Damage < 0.f)
	{
		return 0.f;
	}
	
	// Capture TargetResistanceToDamageType on Target, in order to modify the incoming damage received
	float TargetResistanceToDamageType = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceDamageTypeTag], EvaluateParams, TargetResistanceToDamageType);
	TargetResistanceToDamageType = FMath::Clamp(TargetResistanceToDamageType, 0.f, 100.f);

	// Calculate the final damage by subtracting the resistance to the damage type from the base damage
	Damage *= (100.f - TargetResistanceToDamageType) / 100.f;

	// Handle radial damage
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	if (UAuraAbilitySystemLibrary::GetRadialDamage(EffectContextHandle))
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
		{
			CombatInterface->GetDamageTakenDelegate().AddLambda(
				[&](float DamageAmount)
				{
					Damage = DamageAmount;
				}
			);
		}

		UGameplayStatics::ApplyRadialDamageWithFalloff(
			TargetAvatar,
			Damage,
			0.f,
			UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
			UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
			UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
			1.f,
			UDamageType::StaticClass(),
			TArray<AActor*>(),
			SourceAvatar,
			nullptr
		);
	}
	
	return Damage;
}

/** Determine whether a debuff occured */
void UEC_Damage::HandleDebuff(const FGameplayTag& DamageTypeTag, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& EffectSpec, const FAggregatorEvaluateParameters& EvaluateParams, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();

	const FGameplayTag DebuffTypeTag = *AuraGameplayTags.DamageTypesToDebuffs.Find(DamageTypeTag);
		
	const float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);
	if (DamageTypeValue > -1.f)
	{
		const FGameplayTag& ResistanceDamageTypeTag = AuraGameplayTags.DamageTypesToResistances[DamageTypeTag];

		// Retrieve Source's chance to cause a debuff
		const float SourceDebuffChance = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Chance, false, -1.f);

		// Capture TargetResistanceToDamageType on Target, in order to modify the debuff's chance to occur
		float TargetResistanceToDamageType = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceDamageTypeTag], EvaluateParams, TargetResistanceToDamageType);
		TargetResistanceToDamageType = FMath::Max<float>(TargetResistanceToDamageType, 0.f);

		// Apply Source's debuff chance to the Target, in order to calculate the effective debuff chance based on the Target's resistance to the damage type associated to the debuff
		const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetResistanceToDamageType) / 100.f;

		// Apply the debuff if it was successful
		const bool bDebuff = FMath::RandRange(1.f, 100.f) < EffectiveDebuffChance;
		if (bDebuff)
		{
			const float DebuffDamage = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Damage, false, -1.f);
			const float DebuffDuration = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Duration, false, -1.f);
			const float DebuffFrequency = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags.Debuff_Frequency, false, -1.f);

			FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
			UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(EffectContextHandle, bDebuff);
			UAuraAbilitySystemLibrary::SetDebuffDamage(EffectContextHandle, DebuffDamage);
			UAuraAbilitySystemLibrary::SetDebuffDuration(EffectContextHandle, DebuffDuration);
			UAuraAbilitySystemLibrary::SetDebuffFrequency(EffectContextHandle, DebuffFrequency);
		}
	}
}

/** Check Target's BlockChance in order to calculate Damage taken if there's a successful block */
bool UEC_Damage::HandleBlock(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& OutDamage) const
{
	// Capture BlockChance on Target in order to determine if there was a successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	// Halve Damage's value if it's a block
	const bool bBlocked = FMath::RandRange(1.f, 100.f) < TargetBlockChance;
	OutDamage = bBlocked ? OutDamage / 2.f : OutDamage;

	return bBlocked;
}

/** Check Target's Armor and Source's ArmorPenetration in order to calculate Damage taken */
void UEC_Damage::HandleArmor(const UCharacterClassInfo* CharacterClassInfo, int32 SourceLevel, int32 TargetLevel, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& OutDamage) const
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
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);

	// Retrieve the effective armor's coefficient based on the Target's level
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel);

	// Apply Source's armor penetration and its coefficient to the Target's armor, in order to calculate the effective armor
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	// Calculate final Damage that will be dealt based on the effective armor and its coefficient
	OutDamage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
}

/** Check Target's CriticalHitChance and Source's CriticalHitResistance in order to calculate Damage taken */
bool UEC_Damage::HandleCriticalHit(const UCharacterClassInfo* CharacterClassInfo, int32 TargetLevel, const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, float& OutDamage) const
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
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetLevel);

	// Apply Target's critical hit resistance and its coefficient in order to reduce the critical hit chance
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

	// Double Damage's value and add to its value the Source's CriticalHitDamage if it's a critical hit
	const bool bCriticalHit = FMath::RandRange(1.f, 100.f) < EffectiveCriticalHitChance;
	OutDamage = bCriticalHit ? (OutDamage * 2.f) + SourceCriticalHitDamage : OutDamage;

	return bCriticalHit;
}

#pragma endregion DAMAGE