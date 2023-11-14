// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Fire/AuraFireBolt.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

#pragma region FIREBOLT

/** Event spawn projectile */
void UAuraFireBolt::EventSpawnProjectile(FGameplayEventData Payload)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	const ICombatInterface* AttackingActor = CastChecked<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector SpawnLocation = AttackingActor->GetCombatSocketLocation(CurrentAttackData.CombatSocketTag);
	const FRotator SpawnRotation = (ProjectileTargetLocation - SpawnLocation).Rotation();

	const FVector ForwardVector = SpawnRotation.Vector();
	const int32 NumberOfProjectiles = FMath::Min(GetAbilityLevel(), MaxNumberOfProjectiles);
	TArray<FRotator> ProjectilesRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(ForwardVector, FVector::UpVector, ProjectileSpread, NumberOfProjectiles);

	for (const FRotator& ProjectileRotation : ProjectilesRotations)
	{
		SpawnProjectile(CurrentAttackData.ProjectileClass, SpawnLocation, ProjectileRotation, bOverridePitch, PitchOverride, MouseHitActor);
	}
}

#pragma endregion FIREBOLT

#pragma region DESCRIPTION
	
/** Get ability's description */
FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
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
			ScaledDamage,
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
		ScaledDamage,
		ManaCost,
		Cooldown,
		FMath::Min(MaxNumberOfProjectiles, Level));
}

/** Get ability's description for next level */
FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
		ScaledDamage,
		ManaCost,
		Cooldown,
		FMath::Min(MaxNumberOfProjectiles, Level));
}

#pragma endregion DESCRIPTION