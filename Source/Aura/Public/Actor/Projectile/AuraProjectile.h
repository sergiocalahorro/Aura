// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AuraProjectile.generated.h"

// Forward declarations - Unreal Engine
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;

// Forward declarations - Aura
class UProjectileData;

UCLASS()
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

#pragma endregion OVERRIDES

#pragma region PROJECTILE

protected:

	/** BeginOverlap Callback */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	/** Initialize projectile's values */
	void InitializeProjectile() const;

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
	
};
