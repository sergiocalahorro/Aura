// Copyright Sergio Calahorro

#include "Actor/AuraPotion.h"

// Headers - Unreal Engine
#include "Components/SphereComponent.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraPotion::AAuraPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	
	bDestroyOnEffectsApplication = true;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void AAuraPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAuraPotion::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AAuraPotion::OnEndOverlap);
}

/** Called when the game starts or when spawned */
void AAuraPotion::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES