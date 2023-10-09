// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ProjectileData.generated.h"

// Forward declarations - Unreal Engine
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class AURA_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Life span */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (UIMin = 0.f, ClampMin = 0.f))
	float LifeSpan = 15.f;

	/** Initial speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (UIMin = 0.f, ClampMin = 0.f, ForceUnits = "cm/s"))
	float InitialSpeed = 1000.f;

	/** Maximum speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (UIMin = 0.f, ClampMin = 0.f, ForceUnits = "cm/s"))
	float MaxSpeed = 1000.f;

	/** Gravity scale */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (UIMin = 0.f, ClampMin = 0.f))
	float GravityScale = 0.f;

	/** Whether initial velocity is set in local space */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement")
	bool bInitialVelocityInLocalSpace = true;

	/** Whether rotation follows velocity */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement")
	bool bRotationFollowsVelocity = true;

	/** Whether projectile should bounce */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement")
	bool bShouldBounce = false;
	
	/** Bounciness */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (UIMin = 0.f, ClampMin = 0.f, EditCondition = "bShouldBounce", EditConditionHides))
	float Bounciness = 0.f;
	
	/** Projectile's VFX */
	UPROPERTY(EditDefaultsOnly, Category = "AA|VFX")
	TObjectPtr<UNiagaraSystem> ProjectileVFX;

	/** Projectile's impact VFX */
	UPROPERTY(EditDefaultsOnly, Category = "AA|VFX")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	/** Projectile's looping sound */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Audio")
	TObjectPtr<USoundBase> LoopingSound;

	/** Projectile's impact sound */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Audio")
	TObjectPtr<USoundBase> ImpactSound;
};
