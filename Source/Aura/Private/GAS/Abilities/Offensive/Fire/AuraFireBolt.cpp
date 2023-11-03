// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Fire/AuraFireBolt.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"

#pragma region DESCRIPTION
	
/** Get ability's description */
FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	FString::Printf(TEXT("%s %s", *String1, *String2));

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </>"
			"<Level>%d</>\n"

			// Damage
			"<Small>Damage: </>"
			"<Damage>%d</>\n"

			// Mana Cost
			"<Small>Mana Cost: </>"
			"<ManaCost>%.1f</>\n"

			// Cooldown
			"<Small>Cooldown: </>"
			"<Cooldown>%.1f s</>\n\n"

			// Description
			"<Default>Launches a bolt of fire, exploding on impact and dealing fire damage with a chance to burn.</>"),

			// Values
			Level,
			Damage,
			ManaCost,
			Cooldown);
	}

	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BOLT</>\n\n"

		// Level
		"<Small>Level: </>"
		"<Level>%d</>\n"

		// Damage
		"<Small>Damage: </>"
		"<Damage>%d</>\n"

		// Mana Cost
		"<Small>Mana Cost: </>"
		"<ManaCost>%.1f</>\n"

		// Cooldown
		"<Small>Cooldown: </>"
		"<Cooldown>%.1f s</>\n\n"

		// Description
		"<Default>Launches %d bolts of fire, exploding on impact and dealing fire damage with a chance to burn.</>"),

		// Values
		Level,
		Damage,
		ManaCost,
		Cooldown,
		FMath::Min(NumberOfProjectiles, Level));
}

/** Get ability's description for next level */
FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL: </>\n\n"

		// Level
		"<Small>Level: </>"
		"<Level>%d</>\n"

		// Damage
		"<Small>Damage: </>"
		"<Damage>%d</>\n"

		// Mana Cost
		"<Small>Mana Cost: </>"
		"<ManaCost>%.1f</>\n"

		// Cooldown
		"<Small>Cooldown: </>"
		"<Cooldown>%.1f s</>\n\n"

		// Description
		"<Default>Launches %d bolts of fire, exploding on impact and dealing fire damage with a chance to burn.</>"),

		// Values
		Level,
		Damage,
		ManaCost,
		Cooldown,
		FMath::Min(NumberOfProjectiles, Level));
}

#pragma endregion DESCRIPTION