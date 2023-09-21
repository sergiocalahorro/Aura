// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "Actor/AuraEffectActor.h"

#include "AuraPotion.generated.h"

// Forward declarations - Unreal Engine
class USphereComponent;

UCLASS(Abstract)
class AURA_API AAuraPotion : public AAuraEffectActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AAuraPotion();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES
	
#pragma region COMPONENTS

private:

	/** Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Sphere trigger */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<USphereComponent> Sphere;

#pragma endregion COMPONENTS
	
};
