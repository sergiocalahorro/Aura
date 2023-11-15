// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/Offensive/AuraProjectileSpell.h"

#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

#pragma region FIREBOLT

protected:

	/** Event received for spawning projectile */
	virtual void EventReceivedSpawnProjectile(FGameplayEventData Payload) override;

protected:

	/** Spread between projectiles when multiple projectiles are launched */
	UPROPERTY(EditDefaultsOnly, Category = "AA|FireBolt", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 360.f, UIMax = 360.f, Delta = 1.f, Units = "Degrees"))
	float ProjectileSpread = 90.f;

#pragma endregion FIREBOLT

#pragma region DESCRIPTION
	
public:

	/** Get ability's description */
	virtual FString GetDescription(int32 Level) override;

	/** Get ability's description for next level */
	virtual FString GetNextLevelDescription(int32 Level) override;

#pragma endregion DESCRIPTION
};
