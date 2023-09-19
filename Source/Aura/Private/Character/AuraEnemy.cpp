// Copyright Sergio Calahorro

#include "Character/AuraEnemy.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when the game starts or when spawned */
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES
