// Copyright Sergio Calahorro

#include "GameplayTags/AuraGameplayTags.h"

// Headers - Unreal Engine
#include "GameplayTagsManager.h"

#pragma region CORE

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

/** Initialize native gameplay tags */
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	InitializeNativeAttributesGameplayTags();
	InitializeNativeInputsGameplayTags();
	InitializeNativeAbilitiesGameplayTags();
	InitializeNativeEffectsGameplayTags();
	InitializeNativeDamageGameplayTags();
	InitializeNativeEventsGameplayTags();
	InitializeNativeMessagesGameplayTags();
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
}

#pragma endregion INPUT

#pragma region ABILITIES

/** Initialize native gameplay tags for abilities */
void FAuraGameplayTags::InitializeNativeAbilitiesGameplayTags()
{
	GameplayTags.Abilities_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.FireBolt"), FString("Fire Bolt ability"));
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Attack ability"));
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("Hit React ability"));
}

#pragma endregion ABILITIES

#pragma region EFFECTS

/** Initialize native gameplay tags for effects */
void FAuraGameplayTags::InitializeNativeEffectsGameplayTags()
{
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));
}

#pragma endregion EFFECTS

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

#pragma region EVENTS

/** Initialize native gameplay tags for events */
void FAuraGameplayTags::InitializeNativeEventsGameplayTags()
{
	GameplayTags.Event_Montage_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.FireBolt"), FString("Event sent on the montage used for casting a Fire Bolt"));
	GameplayTags.Event_Montage_Attack_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Attack.Melee"), FString("Event sent on the montage used for performing a Melee Attack"));
}

#pragma endregion EVENTS

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