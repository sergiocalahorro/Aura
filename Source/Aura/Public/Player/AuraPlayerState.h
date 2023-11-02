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

// Forward declarations - Aura
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32);

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

#pragma region OVERRIDES

public:

	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region CORE

public:

	/** Get level */
	int32 GetPlayerLevel() const;

	/** Set level */
	void SetLevel(int32 InLevel);

	/** Add to level */
	void AddToLevel(int32 InLevel);

	/** Get XP */
	int32 GetXP() const;

	/** Set XP */
	void SetXP(int32 InXP);

	/** Add to XP */
	void AddToXP(int32 InXP);

	/** Get AttributePoints */
	int32 GetAttributePoints() const;

	/** Add to AttributePoints */
	void AddToAttributePoints(int32 InAttributePoints);

	/** Get SpellPoints */
	int32 GetSpellPoints() const;

	/** Add to SpellPoints */
	void AddToSpellPoints(int32 InSpellPoints);

private:

	/** Level's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	/** XP's Replication Notify Callback */
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	/** AttributePoints' Replication Notify Callback */
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	/** SpellPoints' Replication Notify Callback */
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

public:

	/** Level up information */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Core")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	/** Delegate called whenever level changes */
	FOnPlayerStatChangedSignature OnLevelChangedDelegate;

	/** Delegate called whenever XP changes */
	FOnPlayerStatChangedSignature OnXPChangedDelegate;

	/** Delegate called whenever attribute points change */
	FOnPlayerStatChangedSignature OnAttributePointsChangedDelegate;

	/** Delegate called whenever spell points change */
	FOnPlayerStatChangedSignature OnSpellPointsChangedDelegate;

private:

	/** Player's level */
	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing = OnRep_Level, Category = "AA|Core")
	int32 Level = 1;
	
	/** Player's XP */
	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing = OnRep_XP, Category = "AA|Core")
	int32 XP = 0;

	/** Player's attribute points */
	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing = OnRep_AttributePoints, Category = "AA|Core")
	int32 AttributePoints = 0;
	
	/** Player's spell points */
	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing = OnRep_SpellPoints, Category = "AA|Core")
	int32 SpellPoints = 0;

#pragma endregion CORE

#pragma region GAS

public:

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the attribute set system component to use for this actor */
	UAttributeSet* GetAttributeSet() const;

protected:
	
	/** Ability system component */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/** Attribute set */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

#pragma endregion GAS
	
};
