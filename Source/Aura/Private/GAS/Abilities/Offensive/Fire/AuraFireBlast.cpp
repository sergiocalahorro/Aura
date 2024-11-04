// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Fire/AuraFireBlast.h"

#include "Actor/Projectile/AuraFireBall.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"

#pragma region OVERRIDES
	
/** Actually activate ability, do not call this directly */
void UAuraFireBlast::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SpawnFireBalls();

	K2_EndAbility();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraFireBlast::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES

#pragma region FIREBLAST

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;

	const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector AvatarForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> FireBallsRotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(AvatarForward, FVector::UpVector, 360.f, NumberOfFireBalls);
	for (const FRotator& SpawnRotation : FireBallsRotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(FireBallClass, SpawnTransform, GetAvatarActorFromActorInfo(), CurrentActorInfo->PlayerController->GetPawn(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		FireBall->DamageEffectParams = MakeDamageEffectParams();
		FireBall->ExplosionDamageEffectParams = MakeDamageEffectParams();
		FireBall->FinishSpawning(SpawnTransform);

		FireBalls.Add(FireBall);
	}
	
	return FireBalls;
}

#pragma endregion FIREBLAST

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