// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "GAS/Effects/EffectProperties.h"

#include "AuraAttributeSet.generated.h"

// Forward declarations - Unreal Engine
struct FGameplayEffectModCallbackData;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values for this object's properties */
	UAuraAttributeSet();
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called just before any modification happens to an attribute */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;

#pragma endregion OVERRIDES

#pragma region ATTRIBUTES

public:

	/** Map that links Gameplay Tags to pointers to Attributes' getters */
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TaggedAttributes;

#pragma region ATTRIBUTES_PRIMARY

public:

	/** Strength's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	/** Intelligence's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	/** Resilience's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	/** Vigor's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

public:

	/** Strength Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "AA|Attributes|Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)

	/** Intelligence Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "AA|Attributes|Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)

	/** Resilience Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "AA|Attributes|Primary")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)

	/** Vigor Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "AA|Attributes|Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)

#pragma endregion ATTRIBUTES_PRIMARY

#pragma region ATTRIBUTES_SECONDARY

public:

	/** Armor's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	/** ArmorPenetration's Replication Notify Callback */
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	/** BlockChance's Replication Notify Callback */
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	/** CriticalHitChance's Replication Notify Callback */
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	/** CriticalHitDamage's Replication Notify Callback */
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	/** CriticalHitResistance's Replication Notify Callback */
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	/** HealthRegeneration's Replication Notify Callback */
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	/** ManaRegeneration's Replication Notify Callback */
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	/** MaxHealth's Replication Notify Callback */
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	/** MaxMana's Replication Notify Callback */
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

public:

	/** Armor Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)

	/** ArmorPenetration Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)

	/** BlockChance Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)

	/** CriticalHitChance Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)

	/** CriticalHitDamage Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)

	/** CriticalHitResistance Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)

	/** HealthRegeneration Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)

	/** ManaRegeneration Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)
	
	/** MaxHealth Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	/** MaxMana Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "AA|Attributes|Secondary")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

#pragma endregion ATTRIBUTES_SECONDARY

#pragma region ATTRIBUTES_RESISTANCE

public:

	/** ResistanceFire's Replication Notify Callback */
	UFUNCTION()
	void OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const;

	/** ResistanceLightning's Replication Notify Callback */
	UFUNCTION()
	void OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const;

	/** ResistanceArcane's Replication Notify Callback */
	UFUNCTION()
	void OnRep_ResistanceArcane(const FGameplayAttributeData& OldResistanceArcane) const;

	/** ResistancePhysical's Replication Notify Callback */
	UFUNCTION()
	void OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const;

public:

	/** ResistanceFire Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceFire, Category = "AA|Attributes|Resistance")
	FGameplayAttributeData ResistanceFire;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ResistanceFire)

	/** ResistanceLightning Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceLightning, Category = "AA|Attributes|Resistance")
	FGameplayAttributeData ResistanceLightning;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ResistanceLightning)

	/** ResistanceArcane Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistanceArcane, Category = "AA|Attributes|Resistance")
	FGameplayAttributeData ResistanceArcane;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ResistanceArcane)

	/** ResistancePhysical Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResistancePhysical, Category = "AA|Attributes|Resistance")
	FGameplayAttributeData ResistancePhysical;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ResistancePhysical)

#pragma endregion ATTRIBUTES_RESISTANCE

#pragma region ATTRIBUTES_VITAL

public:

	/** Health's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	/** Mana's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

public:

	/** Health Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "AA|Attributes|Vital")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)

	/** Mana Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "AA|Attributes|Vital")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

#pragma endregion ATTRIBUTES_VITAL

#pragma region ATTRIBUTES_META

private:

	/** Handle incoming damage */
	void HandleIncomingDamage(const FEffectProperties& EffectProperties);

	/** Show floating damage text above target */
	void ShowFloatingDamageText(const FEffectProperties& EffectProperties, float Damage) const;

public:

	/** IncomingDamage Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage)

#pragma endregion ATTRIBUTES_META

#pragma endregion ATTRIBUTES

#pragma region EFFECTS

private:

	/** Populate structure containing the effect's properties */
	void SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Properties) const;

#pragma endregion EFFECTS
	
};
