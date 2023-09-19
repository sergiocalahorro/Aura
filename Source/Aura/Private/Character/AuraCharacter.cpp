// Copyright Sergio Calahorro

#include "Character/AuraCharacter.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when the game starts or when spawned */
void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES
