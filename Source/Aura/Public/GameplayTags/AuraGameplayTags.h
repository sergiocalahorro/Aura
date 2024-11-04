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

#pragma region ATTRIBUTES_META

public:

	/** Health Attribute */
	FGameplayTag Attributes_Meta_IncomingXP;

#pragma endregion ATTRIBUTES_META

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

	/** Input for passive 1 */
	FGameplayTag InputTag_Passive_1;

	/** Input for passive 2 */
	FGameplayTag InputTag_Passive_2;

#pragma endregion INPUT

#pragma region PLAYER

private:
	
	/** Initialize native gameplay tags for player-related functionality */
	static void InitializeNativePlayerGameplayTags();

public:

	/** Block player's input pressed */
	FGameplayTag Player_Block_InputPressed;

	/** Block player's input held */
	FGameplayTag Player_Block_InputHeld;

	/** Block player's input released */
	FGameplayTag Player_Block_InputReleased;

	/** Block player's cursor trace */
	FGameplayTag Player_Block_CursorTrace;

#pragma endregion PLAYER

#pragma region ABILITIES

public:

	/** Initialize native gameplay tags for abilities */
	static void InitializeNativeAbilitiesGameplayTags();

public:

	/** None ability */
	FGameplayTag Abilities_None;

	/** Attack ability */
	FGameplayTag Abilities_Attack;

	/** Summon ability */
	FGameplayTag Abilities_Summon;

	/** Fire Bolt ability */
	FGameplayTag Abilities_Fire_FireBolt;

	/** Fire Blast ability */
	FGameplayTag Abilities_Fire_FireBlast;

	/** Electrocute ability */
	FGameplayTag Abilities_Lightning_Electrocute;

	/** Arcane Shards ability */
	FGameplayTag Abilities_Arcane_ArcaneShards;

	/** Halo of protection passive ability */
	FGameplayTag Abilities_Passive_HaloOfProtection;

	/** Life siphon passive ability */
	FGameplayTag Abilities_Passive_LifeSiphon;

	/** Mana siphon passive ability */
	FGameplayTag Abilities_Passive_ManaSiphon;

	/** Hit React ability */
	FGameplayTag Abilities_HitReact;

#pragma region ABILITIES_STATUS

public:

	/** Status to represent an ability is locked */
	FGameplayTag Abilities_Status_Locked;

	/** Status to represent an ability is eligible */
	FGameplayTag Abilities_Status_Eligible;

	/** Status to represent an ability is unlocked */
	FGameplayTag Abilities_Status_Unlocked;

	/** Status to represent an ability is equipped */
	FGameplayTag Abilities_Status_Equipped;

#pragma endregion ABILITIES_STATUS

#pragma region ABILITIES_TYPES

public:

	/** Type to represent an offensive ability */
	FGameplayTag Abilities_Types_Offensive;

	/** Type to represent a passive ability */
	FGameplayTag Abilities_Types_Passive;

	/** Type to represent an ability that isn't offensive either passive */
	FGameplayTag Abilities_Types_None;

#pragma endregion ABILITIES_TYPES

#pragma endregion ABILITIES

#pragma region EFFECTS

public:

	/** Initialize native gameplay tags for effects */
	static void InitializeNativeEffectsGameplayTags();

public:

	/** Hit React effect */
	FGameplayTag Effects_HitReact;

#pragma endregion EFFECTS

#pragma region COOLDOWNS

public:

	/** Initialize native gameplay tags for cooldowns */
	static void InitializeNativeCooldownsGameplayTags();

public:

	/** Fire Bolt's cooldown */
	FGameplayTag Cooldown_Fire_FireBolt;

	/** Fire Blast's cooldown */
	FGameplayTag Cooldown_Fire_FireBlast;

	/** Electrocute's cooldown */
	FGameplayTag Cooldown_Lightning_Electrocute;

	/** Arcane Shards' cooldown */
	FGameplayTag Cooldown_Arcane_ArcaneShards;

#pragma endregion COOLDOWNS

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

	/** Map that contains all Damage Types linked to Damage Type Resistances */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

#pragma endregion DAMAGE

#pragma region DEBUFFS

public:

	/** Initialize native gameplay tags for debuffs */
	static void InitializeNativeDebuffsGameplayTags();

public:
	
	/** Burn Debuff */
	FGameplayTag Debuff_Burn;

	/** Stun Debuff */
	FGameplayTag Debuff_Stun;

	/** Arcane Debuff */
	FGameplayTag Debuff_Arcane;

	/** Physical Debuff */
	FGameplayTag Debuff_Physical;

	/** Debuff chance */
	FGameplayTag Debuff_Chance;

	/** Debuff damage */
	FGameplayTag Debuff_Damage;

	/** Debuff duration */
	FGameplayTag Debuff_Duration;

	/** Debuff frequency */
	FGameplayTag Debuff_Frequency;

	/** Map that contains all Damage Types linked to Debuffs */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

#pragma endregion DEBUFFS

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

	/** Montage for Fire Bolt ability */
	FGameplayTag Montage_FireBolt;

	/** Montage for Electrocute ability */
	FGameplayTag Montage_Electrocute;

	/** Montage for Arcane Shards ability */
	FGameplayTag Montage_ArcaneShards;

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

	/** GameplayCue for Electrocute's shock burst */
	FGameplayTag GameplayCue_ShockBurst;

	/** GameplayCue for Electrocute's shock loop */
	FGameplayTag GameplayCue_ShockLoop;

	/** GameplayCue for Arcane Shards */
	FGameplayTag GameplayCue_ArcaneShards;

	/** GameplayCue for Fire Blast */
	FGameplayTag GameplayCue_FireBlast;

	/** GameplayCue for Halo of protection's passive ability */
	FGameplayTag GameplayCue_Passive_HaloOfProtection;

	/** GameplayCue for Life siphon's passive ability */
	FGameplayTag GameplayCue_Passive_LifeSiphon;

	/** GameplayCue for Mana siphon's passive ability */
	FGameplayTag GameplayCue_Passive_ManaSiphon;

	/** GameplayCue for burn debuff */
	FGameplayTag GameplayCue_Debuff_Burn;

	/** GameplayCue for stun debuff */
	FGameplayTag GameplayCue_Debuff_Stun;

	/** GameplayCue for arcane debuff */
	FGameplayTag GameplayCue_Debuff_Arcane;

	/** GameplayCue for physical debuff */
	FGameplayTag GameplayCue_Debuff_Physical;

	/** Map that contains all Debuffs linked to Cues */
	TMap<FGameplayTag, FGameplayTag> DebuffsToCues;

#pragma endregion CUES

};