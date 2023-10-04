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

#pragma region CORE

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

#pragma endregion CORE

#pragma region ATTRIBUTES

private:

	/** Initialize native gameplay tags for attributes */
	static void InitializeNativeAttributesGameplayTags();

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

#pragma region INPUT

private:
	
	/** Initialize native gameplay tags for inputs */
	static void InitializeNativeInputsGameplayTags();

public:

	/** Input LMB */
	FGameplayTag InputTag_LMB;
	
	/** Input RMB */
	FGameplayTag InputTag_RMB;

	/** Input 1 key */
	FGameplayTag InputTag_1;

	/** Input 2 key */
	FGameplayTag InputTag_2;

	/** Input 3 key */
	FGameplayTag InputTag_3;

	/** Input 4 key */
	FGameplayTag InputTag_4;

#pragma endregion INPUT
};