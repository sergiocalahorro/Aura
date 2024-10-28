// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Fire/AuraFireBlast.h"

#pragma region DESCRIPTION
	
/** Get ability's description */
FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BLAST</>\n\n"

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
		"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, "
		"causing radial fire damage with a chance to burn.</>"),

		// Values
		Level,
		ScaledDamage,
		ManaCost,
		Cooldown,
		NumberOfFireBalls);
}

/** Get ability's description for next level */
FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL:</>\n\n"

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
		"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, "
		"causing radial fire damage with a chance to burn.</>"),

		// Values
		Level,
		ScaledDamage,
		ManaCost,
		Cooldown,
		NumberOfFireBalls);
}

#pragma endregion DESCRIPTION