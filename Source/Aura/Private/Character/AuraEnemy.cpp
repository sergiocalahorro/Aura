﻿// Copyright Sergio Calahorro

#include "Character/AuraEnemy.h"

// Headers - Aura
#include "Aura.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Attributes/AuraAttributeSet.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// GAS setup
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when the game starts or when spawned */
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

#pragma endregion OVERRIDES

#pragma region COMBAT

/** Get level */
int32 AAuraEnemy::GetCurrentLevel() const
{
	return Level;
}

#pragma endregion COMBAT

#pragma region INTERACTABLE

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

#pragma endregion INTERACTABLE

#pragma region GAS

/** Initialize ability actor info */
void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	ApplyEffectDefinitionsToSelf(DefaultEffects);
}

#pragma endregion GAS
