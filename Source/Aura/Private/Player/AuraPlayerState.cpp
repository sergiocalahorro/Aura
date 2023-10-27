// Copyright Sergio Calahorro

#include "Player/AuraPlayerState.h"

// Headers - Unreal Engine
#include "Net/UnrealNetwork.h"

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

#pragma region OVERRIDES

/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

#pragma endregion OVERRIDES

#pragma region CORE

/** Get level */
int32 AAuraPlayerState::GetCurrentLevel() const
{
	return Level;
}

/** Set level */
void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

/** Add to level */
void AAuraPlayerState::AddToLevel(int32 LevelToAdd)
{
	Level += LevelToAdd;
	OnLevelChangedDelegate.Broadcast(Level);
}

/** Get XP */
int32 AAuraPlayerState::GetXP() const
{
	return XP;
}

/** Set XP */
void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

/** Add to XP */
void AAuraPlayerState::AddToXP(int32 XPToAdd)
{
	XP += XPToAdd;
	OnXPChangedDelegate.Broadcast(XP);
}

/** Level's Replication Notify Callback */
void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

/** XP's Replication Notify Callback */
void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

#pragma endregion CORE

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