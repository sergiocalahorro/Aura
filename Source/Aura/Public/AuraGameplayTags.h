// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FAuraGameplayTags
{
	
public:

	/** Get instance */
    static const FAuraGameplayTags& Get()
	{
		return GameplayTags;
	}

	/** Initialize native gameplay tags */
	static void InitializeNativeGameplayTags();

private:

	/** Gameplay tags' instance */
    static FAuraGameplayTags GameplayTags;

#pragma region ATTRIBUTES

#pragma region ATTRIBUTES_PRIMARY

public:

	/** Strength Attribute */
	FGameplayTag Attributes_Primary_Strength;

	/** Intelligence Attribute */
	FGameplayTag Attributes_Primary_Intelligence;

	/** Resilience Attribute */
	FGameplayTag Attributes_Primary_Resilience;

	/** Vigor Attribute */
	FGameplayTag Attributes_Primary_Vigor;

#pragma endregion ATTRIBUTES_PRIMARY
	
#pragma region ATTRIBUTES_SECONDARY

public:

	/** Armor Attribute */
	FGameplayTag Attributes_Secondary_Armor;

	/** ArmorPenetration Attribute */
	FGameplayTag Attributes_Secondary_ArmorPenetration;

	/** BlockChance Attribute */
	FGameplayTag Attributes_Secondary_BlockChance;

	/** CriticalHitChance Attribute */
	FGameplayTag Attributes_Secondary_CriticalHitChance;

	/** CriticalHitDamage Attribute */
	FGameplayTag Attributes_Secondary_CriticalHitDamage;

	/** CriticalHitResistance Attribute */
	FGameplayTag Attributes_Secondary_CriticalHitResistance;

	/** HealthRegeneration Attribute */
	FGameplayTag Attributes_Secondary_HealthRegeneration;

	/** ManaRegeneration Attribute */
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	
	/** MaxHealth Attribute */
	FGameplayTag Attributes_Secondary_MaxHealth;

	/** MaxMana Attribute */
	FGameplayTag Attributes_Secondary_MaxMana;

#pragma endregion ATTRIBUTES_SECONDARY

#pragma endregion ATTRIBUTES
};