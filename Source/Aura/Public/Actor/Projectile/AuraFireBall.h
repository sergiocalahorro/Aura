// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraProjectile.h"

#include "AuraFireBall.generated.h"

UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region PROJECTILE

protected:
	
	/** BeginOverlap Callback */
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/** Functionality performed when projectile hits something */
	virtual void ProjectileHit() override;

#pragma endregion PROJECTILE

#pragma region FIREBALL

public:

	/** Start fire ball's movement */
	UFUNCTION(BlueprintImplementableEvent)
	void StartMovement();

private:

	/** Distance the fire ball will travel to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|FireBall", meta = (AllowPrivateAccess = "true", Units = "Centimeters"))
	float TravelDistance = 800.f;

	/** Distance from the instigator at which the fire ball will explode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|FireBall", meta = (AllowPrivateAccess = "true", Units = "Centimeters"))
	float ExplodeDistance = 150.f;

#pragma endregion FIREBALL

#pragma region EXPLOSION

public:

	/** Fire ball's explosion */
	UFUNCTION(BlueprintCallable)
	void Explosion();

public:

	/** Explosion's damage params */
	FDamageEffectParams ExplosionDamageEffectParams;

protected:

	/** Explosion's inner radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Explosion", meta = (UIMin = 0.f, ClampMin = 0.f, Units = "Centimeters"))
	float ExplosionInnerRadius = 50.f;

	/** Explosion's outer radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Explosion", meta = (UIMin = 0.f, ClampMin = 0.f, Units = "Centimeters"))
	float ExplosionOuterRadius = 300.f;

	/** Explosion's knockback magnitude */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Explosion", meta = (UIMin = 0.f, ClampMin = 0.f, Units = "Centimeters"))
	float ExplosionKnockbackMagnitude = 800.f;

	/** Explosion's death impulse magnitude */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Explosion", meta = (UIMin = 0.f, ClampMin = 0.f, Units = "Centimeters"))
	float ExplosionDeathImpulseMagnitude = 600.f;
	
#pragma endregion EXPLOSION
};
