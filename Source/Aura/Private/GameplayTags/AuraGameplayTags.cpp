﻿// Copyright Sergio Calahorro

#include "GameplayTags/AuraGameplayTags.h"

// Headers - Unreal Engine
#include "GameplayCueManager.h"
#include "GameplayTagsManager.h"

#pragma region CORE

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

/** Initialize native gameplay tags */
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	InitializeNativeAttributesGameplayTags();
	InitializeNativeInputsGameplayTags();
	InitializeNativePlayerGameplayTags();
	InitializeNativeAbilitiesGameplayTags();
	InitializeNativeEffectsGameplayTags();
	InitializeNativeCooldownsGameplayTags();
	InitializeNativeDamageGameplayTags();
	InitializeNativeDebuffsGameplayTags();
	InitializeNativeCombatSocketsGameplayTags();
	InitializeNativeMontagesGameplayTags();
	InitializeNativeMessagesGameplayTags();
	InitializeNativeCuesGameplayTags();
}

#pragma endregion CORE

#pragma region ATTRIBUTES

/** Initialize native gameplay tags for attributes */
void FAuraGameplayTags::InitializeNativeAttributesGameplayTags()
{
	// Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases Armor and Armor Penetration"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases Health"));
	
	// Secondary Attributes
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Ignored Percentage of enemy Armor, increases Critical Hit Chance"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to double damage plus critical hit bonus"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus damage added to critical hits"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces Critical Hit Chance of attacking enemies"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of Health regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of Mana regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Maximum amount of Mana obtainable"));
	
	// Damage Type Resistances Attributes
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Resistance to Fire Damage Type"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning Damage Type"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Resistance to Arcane Damage Type"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Resistance to Physical Damage Type"));
	
	// Vital Attributes
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString("Amount of damage a character can take before death"));
	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"), FString("A resource used to cast spells"));

	// Meta Attributes
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("Incoming amount of XP"));
}

#pragma endregion ATTRIBUTES

#pragma region INPUT

/** Initialize native gameplay tags for inputs */
void FAuraGameplayTags::InitializeNativeInputsGameplayTags()
{
	// Mouse input
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag for Right Mouse Button"));

	// Keyboard input
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag for 1 key"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag for 2 key"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input Tag for 3 key"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input Tag for 4 key"));

	// Passive abilities
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"), FString("Input Tag for passive 1"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"), FString("Input Tag for passive 2"));
}

#pragma endregion INPUT

#pragma region PLAYER

/** Initialize native gameplay tags for player-related functionality */
void FAuraGameplayTags::InitializeNativePlayerGameplayTags()
{
	// Block
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), FString("Block player's input pressed"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), FString("Block player's input held"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), FString("Block player's input released"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Block player's cursor trace"));
}

#pragma endregion PLAYER

#pragma region ABILITIES

/** Initialize native gameplay tags for abilities */
void FAuraGameplayTags::InitializeNativeAbilitiesGameplayTags()
{
	// Abilities
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString("No ability (null)"));
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Attack ability"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Summon ability"));

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("Fire Bolt ability"));
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("Fire Blast ability"));
	
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Electrocute ability"));

	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), FString("Arcane Shards ability"));

	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HaloOfProtection"), FString("Halo of protection passive ability"));
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.LifeSiphon"), FString("Life siphon passive ability"));
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaSiphon"), FString("Mana siphon passive ability"));

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("Hit React ability"));

	// Status
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Ability in locked status"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Ability in eligible status"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Ability in unlocked status"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Ability in equipped status"));
	
	// Types
	GameplayTags.Abilities_Types_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Types.Offensive"), FString("Offensive ability type"));
	GameplayTags.Abilities_Types_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Types.Passive"), FString("Passive ability type"));
	GameplayTags.Abilities_Types_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Types.None"), FString("Normal ability type"));
}

#pragma endregion ABILITIES

#pragma region EFFECTS

/** Initialize native gameplay tags for effects */
void FAuraGameplayTags::InitializeNativeEffectsGameplayTags()
{
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));
}

#pragma endregion EFFECTS

#pragma region COOLDOWNS

/** Initialize native gameplay tags for cooldowns */
void FAuraGameplayTags::InitializeNativeCooldownsGameplayTags()
{
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("Fire Bolt's ability cooldown"));
	GameplayTags.Cooldown_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBlast"), FString("Fire Blast's ability cooldown"));

	GameplayTags.Cooldown_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lightning.Electrocute"), FString("Electrocute's ability cooldown"));

	GameplayTags.Cooldown_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Arcane.ArcaneShards"), FString("Arcane Shards' ability cooldown"));
}

#pragma endregion COOLDOWNS

#pragma region DAMAGE

/** Initialize native gameplay tags for damage */
void FAuraGameplayTags::InitializeNativeDamageGameplayTags()
{
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage Type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage Type"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Arcane Damage Type"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage Type"));

	// Map Damage Types to their resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
}

#pragma endregion DAMAGE

#pragma region DEBUFFS

void FAuraGameplayTags::InitializeNativeDebuffsGameplayTags()
{
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"), FString("Burn Debuff"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"), FString("Stun Debuff"));
	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"), FString("Arcane Debuff"));
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"), FString("Physical Debuff"));

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"), FString("Debuff chance"));
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"), FString("Debuff damage"));
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"), FString("Debuff duration"));
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"), FString("Debuff frequency"));
	
	// Map Damage Types to their debuffs
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
}

#pragma endregion DEBUFFS

#pragma region COMBAT_SOCKETS

/** Initialize native gameplay tags for combat sockets */
void FAuraGameplayTags::InitializeNativeCombatSocketsGameplayTags()
{
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Socket for attacks with weapon"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Socket for attacks with right hand"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Socket for attacks with left hand"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("Socket for attacks with tail"));
}

#pragma endregion COMBAT_SOCKETS

#pragma region MONTAGES

/** Initialize native gameplay tags for montages */
void FAuraGameplayTags::InitializeNativeMontagesGameplayTags()
{
	GameplayTags.Montage_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.FireBolt"), FString("Montage for Fire Bolt ability"));

	GameplayTags.Montage_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Electrocute"), FString("Montage for Electrocute ability"));

	GameplayTags.Montage_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ArcaneShards"), FString("Montage for Arcane Shards ability"));

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage for Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage for Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage for Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage for Attack 4"));
}
	
#pragma endregion MONTAGES

#pragma region MESSAGES

/** Initialize native gameplay tags for events */
void FAuraGameplayTags::InitializeNativeMessagesGameplayTags()
{
	GameplayTags.Message_HealthPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.HealthPotion"), FString("Message bound to picking up a Health Potion"));
	GameplayTags.Message_ManaPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.ManaPotion"), FString("Message bound to picking up a Mana Potion"));
	GameplayTags.Message_HealthCrystal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.HealthCrystal"), FString("Message bound to picking up a Health Crystal"));
	GameplayTags.Message_ManaCrystal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.ManaCrystal"), FString("Message bound to picking up a Mana Crystal"));
}

#pragma endregion MESSAGES

#pragma region CUES

/** Initialize native gameplay tags for GameplayCues */
void FAuraGameplayTags::InitializeNativeCuesGameplayTags()
{
	GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.MeleeImpact"), FString("GameplayCue for Melee Attacks' impacts"));

	GameplayTags.GameplayCue_ShockBurst = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ShockBurst"), FString("GameplayCue for Electrocute's shock burst"));
	GameplayTags.GameplayCue_ShockLoop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ShockLoop"), FString("GameplayCue for Electrocute's shock loop"));

	GameplayTags.GameplayCue_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ArcaneShards"), FString("GameplayCue for Arcane Shards"));

	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"), FString("GameplayCue for Fire Blast"));
	
	GameplayTags.GameplayCue_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Passive.HaloOfProtection"), FString("GameplayCue for Halo of protection's passive ability"));
	GameplayTags.GameplayCue_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Passive.LifeSiphon"), FString("GameplayCue for Life siphon's passive ability"));
	GameplayTags.GameplayCue_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Passive.ManaSiphon"), FString("GameplayCue for Mana siphon's passive ability"));
	
	GameplayTags.GameplayCue_Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Debuff.Burn"), FString("GameplayCue for burn debuff"));
	GameplayTags.GameplayCue_Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Debuff.Stun"), FString("GameplayCue for stun debuff"));
	GameplayTags.GameplayCue_Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Debuff.Arcane"), FString("GameplayCue for arcane debuff"));
	GameplayTags.GameplayCue_Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Debuff.Physical"), FString("GameplayCue for physical debuff"));

	// Map Debuffs to their cues
	GameplayTags.DebuffsToCues.Add(GameplayTags.Debuff_Burn, GameplayTags.GameplayCue_Debuff_Burn);
	GameplayTags.DebuffsToCues.Add(GameplayTags.Debuff_Stun, GameplayTags.GameplayCue_Debuff_Stun);
	GameplayTags.DebuffsToCues.Add(GameplayTags.Debuff_Arcane, GameplayTags.GameplayCue_Debuff_Arcane);
	GameplayTags.DebuffsToCues.Add(GameplayTags.Debuff_Physical, GameplayTags.GameplayCue_Debuff_Physical);
}

#pragma endregion CUES