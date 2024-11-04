// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS/Utils/AuraAbilityTypes.h"

#include "AuraProjectile.generated.h"

// Forward declarations - Unreal Engine
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;

// Forward declarations - Aura
class UProjectileData;

UCLASS(Abstract)
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AAuraProjectile();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	
	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when this actor is explicitly being destroyed during gameplay or in the editor, not called during level streaming or gameplay ending */
	virtual void Destroyed() override;

#pragma endregion OVERRIDES

#pragma region PROJECTILE

protected:

	/** BeginOverlap Callback */
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/** Returns whether overlap is valid with other actor */
	bool IsOverlapValid(AActor* OtherActor) const;

	/** Functionality performed when projectile hits something */
	virtual void ProjectileHit();
	
private:

	/** Initialize projectile's values */
	void InitializeProjectile() const;

public:

	/** Damage effect's params*/
	UPROPERTY()
	FDamageEffectParams DamageEffectParams;

protected:

	/** Whether the projectile has hit something */
	bool bHit;

	/** Audio component for looping sound */
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

private:

	/** Sphere collision */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<USphereComponent> Sphere;

	/** Projectile's VFX */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<UNiagaraComponent> ProjectileVFX;

	/** Projectile's movement component */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	/** Projectile's data */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<UProjectileData> ProjectileData;

#pragma endregion PROJECTILE

#pragma region HOMING

public:

	/** Set projectile's homing target */
	void SetHomingTarget(AActor* HomingTarget);

	/** Set projectile's homing target location */
	void SetHomingTargetLocation(const FVector& TargetLocation);

private:

	/** Set projectile's homing behaviour */
	void SetHomingBehaviour() const;

	/** Functionality performed once the homing target is destroyed */
	UFUNCTION()
	void OnHomingTargetDestroyed(AActor* DeadActor);

private:
	
	/** Homing target's scene component */
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetComponent;

#pragma endregion HOMING
	
};
