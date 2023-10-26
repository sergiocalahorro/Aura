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

#pragma region ATTRIBUTES_RESISTANCE

public:

	/** Fire Resistance Attribute */
	FGameplayTag Attributes_Resistance_Fire;

	/** Lightning Resistance Attribute */
	FGameplayTag Attributes_Resistance_Lightning;

	/** Arcane Resistance Attribute */
	FGameplayTag Attributes_Resistance_Arcane;

	/** Physical Resistance Attribute */
	FGameplayTag Attributes_Resistance_Physical;

#pragma endregion ATTRIBUTES_RESISTANCE

#pragma region ATTRIBUTES_VITAL

public:

	/** Health Attribute */
	FGameplayTag Attributes_Vital_Health;

	/** Mana Attribute */
	FGameplayTag Attributes_Vital_Mana;

#pragma endregion ATTRIBUTES_VITAL

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

#pragma region ABILITIES

public:

	/** Initialize native gameplay tags for abilities */
	static void InitializeNativeAbilitiesGameplayTags();

public:

	/** Attack ability */
	FGameplayTag Abilities_Attack;

	/** Summon ability */
	FGameplayTag Abilities_Summon;

	/** Fire Bolt ability */
	FGameplayTag Abilities_Fire_FireBolt;

	/** Hit React ability */
	FGameplayTag Abilities_HitReact;

#pragma region ABILITIES

#pragma region EFFECTS

public:

	/** Initialize native gameplay tags for effects */
	static void InitializeNativeEffectsGameplayTags();

public:

	/** Hit React effect */
	FGameplayTag Effects_HitReact;

#pragma region EFFECTS

#pragma region DAMAGE

public:

	/** Initialize native gameplay tags for damage */
	static void InitializeNativeDamageGameplayTags();

public:

	/** Damage */
	FGameplayTag Damage;

	/** Fire Damage Type */
	FGameplayTag Damage_Fire;

	/** Lightning Damage Type */
	FGameplayTag Damage_Lightning;

	/** Arcane Damage Type */
	FGameplayTag Damage_Arcane;

	/** Physical Damage Type */
	FGameplayTag Damage_Physical;

	/** Array that contains all Damage Types linked to Damage Type Resistances */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

#pragma region DAMAGE

#pragma region COMBAT_SOCKETS

private:
	
	/** Initialize native gameplay tags for combat sockets */
	static void InitializeNativeCombatSocketsGameplayTags();

public:

	/** Socket for attacks with weapon */
	FGameplayTag CombatSocket_Weapon;

	/** Socket for attacks with right hand */
	FGameplayTag CombatSocket_RightHand;

	/** Socket for attacks with left hand */
	FGameplayTag CombatSocket_LeftHand;

	/** Socket for attacks with tail */
	FGameplayTag CombatSocket_Tail;
	
#pragma endregion COMBAT_SOCKETS

#pragma region MONTAGES

private:
	
	/** Initialize native gameplay tags for montages */
	static void InitializeNativeMontagesGameplayTags();

public:

	/** Montage for Attack 1 */
	FGameplayTag Montage_Attack_1;

	/** Montage for Attack 2 */
	FGameplayTag Montage_Attack_2;

	/** Montage for Attack 3 */
	FGameplayTag Montage_Attack_3;

	/** Montage for Attack 4 */
	FGameplayTag Montage_Attack_4;
	
#pragma endregion MONTAGES

#pragma region MESSAGES

private:
	
	/** Initialize native gameplay tags for messages */
	static void InitializeNativeMessagesGameplayTags();

public:

	/** Message bound to picking up a Health Potion */
	FGameplayTag Message_HealthPotion;

	/** Message bound to picking up a Mana Potion */
	FGameplayTag Message_ManaPotion;

	/** Message bound to picking up a Health Crystal */
	FGameplayTag Message_HealthCrystal;

	/** Message bound to picking up a Mana Crystal */
	FGameplayTag Message_ManaCrystal;

#pragma endregion MESSAGES

#pragma region CUES

private:
	
	/** Initialize native gameplay tags for GameplayCues */
	static void InitializeNativeCuesGameplayTags();

public:

	/** GameplayCue for Melee Attacks' impacts */
	FGameplayTag GameplayCue_MeleeImpact;

#pragma endregion CUES

};