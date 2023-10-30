// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraBaseCharacter.h"
#include "Interaction/PlayerInterface.h"

#include "AuraCharacter.generated.h"

// Forward declarations - Unreal Engine
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacter : public AAuraBaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this character's properties */
	AAuraCharacter();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called when this Pawn is possessed. Only called on the server (or in standalone) */
	virtual void PossessedBy(AController* NewController) override;

	/** PlayerState Replication Notification Callback. Only called on clients */
	virtual void OnRep_PlayerState() override;
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

private:

	/** Camera */
	UPROPERTY(EditAnywhere, Category = "AA|Components")
	TObjectPtr<UCameraComponent> Camera;
	
	/** Spring Arm */
	UPROPERTY(EditAnywhere, Category = "AA|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	/** Level Up Niagara component */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

#pragma endregion COMPONENTS

#pragma region COMBAT

public:

	/** Get level */
	virtual int32 GetCurrentLevel_Implementation() override;

#pragma endregion COMBAT

#pragma region PLAYER

public:

	/** Add incoming XP to accumulated XP */
	virtual void AddToXP_Implementation(int32 InXP) override;

	/** Get current accumulated XP */
	virtual int32 GetXP_Implementation() const override;

	/** Get attribute points reward for new level */
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;

	/** Get spell points reward for new level */
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;

	/** Add incoming level to player's current level */
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;

	/** Add incoming attribute points to player's current attribute points */
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

	/** Add incoming spell points to player's current spell points */
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

	/** Get player's attribute points */
	virtual int32 GetAttributePoints_Implementation() const override;

	/** Get player's spell points */
	virtual int32 GetSpellPoints_Implementation() const override;

	/** Handle level up */
	virtual void LevelUp_Implementation() override;

	/** Find level for incoming XP */
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;

private:

	/** Multicast RPC for displaying level up particles */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

#pragma endregion PLAYER

#pragma region GAS

protected:

	/** Initialize ability actor info */
	virtual void InitAbilityActorInfo() override;

private:

	/** Effect used for initializing Primary Attributes' values */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	/** Effect used for initializing Secondary Attributes' values */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	/** Effect used for initializing Vital Attributes' values */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TSubclassOf<UGameplayEffect> VitalAttributes;

#pragma endregion GAS
	
};
