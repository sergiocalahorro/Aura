// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "AuraPlayerState.generated.h"

// Forward declarations - Unreal Engine
class UAttributeSet;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this actor's properties */
	AAuraPlayerState();

#pragma endregion INITIALIZATION

#pragma region GAS

public:

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the attribute set system component to use for this actor */
	UAttributeSet* GetAttributeSet() const;

protected:
	
	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/** Attribute set */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

#pragma endregion GAS
	
};
