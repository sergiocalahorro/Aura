// Copyright Sergio Calahorro

#include "Actor/AuraArea.h"

// Headers - Unreal Engine
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraArea::AAuraArea()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AreaEffect"));
	AreaEffect->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void AAuraArea::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Box->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAuraArea::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddUniqueDynamic(this, &AAuraArea::OnEndOverlap);
}

/** Called when the game starts or when spawned */
void AAuraArea::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES