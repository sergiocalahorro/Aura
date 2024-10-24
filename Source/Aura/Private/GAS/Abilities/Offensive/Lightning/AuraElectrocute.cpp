// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Lightning/AuraElectrocute.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraElectrocute::UAuraElectrocute()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#pragma endregion INITIALIZATION

#pragma region DESCRIPTION
	
/** Get ability's description */
FString UAuraElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ELECTROCUTE</>\n\n"

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
			"<Default>Emits a beam of lightning, connecting with the target, repeatedly causing damage with a chance to stun</>"),

			// Values
			Level,
			ScaledDamage,
			ManaCost,
			Cooldown);
	}

	return FString::Printf(TEXT(
		// Title
		"<Title>ELECTROCUTE</>\n\n"

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
		"<Default>Emits a beam of lightning, propagating to %d additional targets nearby, repeatedly causing damage with a chance to stun.</>"),

		// Values
		Level,
		ScaledDamage,
		ManaCost,
		Cooldown,
		FMath::Min(MaxPropagationTargets - 1, Level));
}

/** Get ability's description for next level */
FString UAuraElectrocute::GetNextLevelDescription(int32 Level)
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
	"<Default>Emits a beam of lightning, propagating to %d additional targets nearby, repeatedly causing damage with a chance to stun.</>"),

	// Values
	Level,
	ScaledDamage,
	ManaCost,
	Cooldown,
	FMath::Min(MaxPropagationTargets - 1, Level));
}

#pragma endregion DESCRIPTION