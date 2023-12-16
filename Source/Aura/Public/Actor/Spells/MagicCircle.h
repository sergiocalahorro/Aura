// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MagicCircle.generated.h"

UCLASS(Abstract)
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AMagicCircle();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

public:

	/** Set magic circle's decal material */
	void SetDecalMaterial(UMaterialInterface* DecalMaterial) const;

protected:

	/** Magic circle's decal */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UDecalComponent> MagicCircleDecal;

#pragma endregion COMPONENTS
	
};
