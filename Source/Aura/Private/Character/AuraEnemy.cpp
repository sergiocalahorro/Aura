﻿// Copyright Sergio Calahorro

#include "Character/AuraEnemy.h"

// Headers - Aura
#include "Aura.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when the game starts or when spawned */
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region HIGHLIGHTABLE

/** Functionality performed when Actor should be highlighted (on hovered) */
void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AAuraEnemy::HighlightActor - %s"), *GetNameSafe(this));

	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

/** Functionality performed when Actor should be un-highlighted (stopped being hovered) */
void AAuraEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AAuraEnemy::UnHighlightActor - %s"), *GetNameSafe(this));

	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

#pragma endregion HIGHLIGHTABLE
