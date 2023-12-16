// Copyright Sergio Calahorro

#include "Actor/Spells/MagicCircle.h"

// Headers - Unreal Engine
#include "Components/DecalComponent.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MagicCircleDecal"));
	MagicCircleDecal->SetupAttachment(RootComponent);
	MagicCircleDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	MagicCircleDecal->SetWorldScale3D(FVector(0.75f, 1.f, 1.f));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region COMPONENTS

/** Set magic circle's decal material */
void AMagicCircle::SetDecalMaterial(UMaterialInterface* DecalMaterial) const
{
	MagicCircleDecal->SetDecalMaterial(DecalMaterial);
}

#pragma endregion COMPONENTS
