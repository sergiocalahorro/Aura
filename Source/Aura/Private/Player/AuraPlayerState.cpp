// Copyright Sergio Calahorro

#include "Player/AuraPlayerState.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Attributes/AuraAttributeSet.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraPlayerState::AAuraPlayerState()
{
	// GAS setup
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
	
	NetUpdateFrequency = 100.f;
}

#pragma endregion INITIALIZATION

#pragma region GAS

/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/** Returns the attribute set system component to use for this actor */
UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

#pragma endregion GAS