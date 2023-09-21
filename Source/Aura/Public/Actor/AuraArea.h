// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "Actor/AuraEffectActor.h"

#include "AuraArea.generated.h"

// Forward declarations - Unreal Engine
class UBoxComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class AURA_API AAuraArea : public AAuraEffectActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AAuraArea();

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

	/** Area Effect */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UNiagaraComponent> AreaEffect;

	/** Sphere trigger */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UBoxComponent> Box;

#pragma endregion COMPONENTS
	
};
