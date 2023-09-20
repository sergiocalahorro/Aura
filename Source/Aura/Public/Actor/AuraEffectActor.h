// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AuraEffectActor.generated.h"

// Forward declarations - Unreal Engine
class USphereComponent;

UCLASS(Abstract)
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AAuraEffectActor();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

protected:
#pragma endregion OVERRIDES
	
#pragma region COMPONENTS

private:

	/** Mesh */
	UPROPERTY(VisibleAnywhere, Category = "AA|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Sphere trigger */
	UPROPERTY(VisibleAnywhere, Category = "AA|Components")
	TObjectPtr<USphereComponent> Sphere;

#pragma endregion COMPONENTS

#pragma region CORE

private:

	/** Sphere's BeginOverlap Callback */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Sphere's EndOverlap Callback */
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
#pragma endregion CORE
	
};
