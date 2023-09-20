﻿// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraBaseCharacter.h"

#include "AuraCharacter.generated.h"

// Forward declarations - Unreal Engine
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacter : public AAuraBaseCharacter
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

#pragma endregion COMPONENTS

#pragma region GAS

private:

	/** Initialize ability actor info */
	void InitAbilityActorInfo();

#pragma endregion GAS
	
};