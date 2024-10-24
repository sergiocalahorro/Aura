﻿// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "AuraPlayerController.generated.h"

// Forward declarations - Unreal Engine
class UInputMappingContext;
class UInputAction;
class USplineComponent;
class UNiagaraSystem;
struct FInputActionValue;
struct FGameplayTag;

// Forward declarations - Aura
class IInteractableInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class UDamageTextWidgetComponent;
class AMagicCircle;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this actor's properties */
	AAuraPlayerController();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation() */
	virtual void PlayerTick(float DeltaTime) override;

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings */
	virtual void SetupInputComponent() override;

#pragma endregion OVERRIDES

#pragma region INPUT

private:
	
	/** Called for moving the player */
	void Move(const FInputActionValue& InputActionValue);

	/** Called when the Shift key is pressed */
	void ShiftPressed();

	/** Called when the Shift key is released */
	void ShiftReleased();

	/** Callback for Input Pressed */
	void AbilityInputTagPressed(FGameplayTag InputTag);

	/** Callback for Input Held */
	void AbilityInputTagHeld(FGameplayTag InputTag);

	/** Callback for Input Released */
	void AbilityInputTagReleased(FGameplayTag InputTag);

private:

	/** Input Mapping Context */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputMappingContext> AuraMappingContext;

	/** Input Action Move */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Input Action Shift */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> ShiftAction;

	/** Input Config */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	/** Whether Shift key is pressed */
	bool bShiftKeyPressed;

#pragma endregion INPUT

#pragma region MOVEMENT

private:

	/** Generate path to the location under the mouse cursor, after a short press occured */
	void StartAutoRun();

	/** Move pawn automatically following a path to the location under the mouse cursor, after a short press occured */
	void AutoRun();

	/** Follow mouse cursor to move pawn */
	void FollowMouseCursor();

private:

	/** Destination where the pawn should move to */
	FVector CachedDestination;

	/** Time the pawn is following the mouse cursor (time until the input was released after being pressed) */
	float FollowTime;

	/** Threshold for considering the time the input was pressed as a short press */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.f, UIMax = 1.f, Delta = 0.01f))
	float ShortPressThreshold = 0.5f;

	/** Whether the pawn is auto-running */
	bool bAutoRunning;

	/** Whether an Actor is targeted (highlighted), used for choosing between performing ability logic/moving the pawn */
	bool bTargeting;

	/** Radius of acceptance for considering the pawn reached its destination (and therefore auto-run should be stopped) */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement", meta = (ClampMin = 0.f, UIMin = 0.f, Delta = 0.5f))
	float AutoRunAcceptanceRadius = 50.f;

	/** Spline used for auto-running */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	/** Particles spawned when clicking */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement")
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

#pragma endregion MOVEMENT

#pragma region INTERACTABLE

private:
	
	/** Trace hit under cursor to highlight Actors */
	void CursorTrace(float DeltaTime);

private:

	/** Cursor's trace channel */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Interactable")
	TEnumAsByte<ECollisionChannel> CursorTraceChannel = ECC_Visibility;

	/** Last Actor that was highlighted */
	IInteractableInterface* LastHighlightedActor;

	/** Current Actor that is highlighted */
	IInteractableInterface* CurrentHighlightedActor;
	
	/** Hit result containing information about the trace performed under the cursor */
	FHitResult CursorHit;

#pragma endregion INTERACTABLE

#pragma region COMBAT

public:

	/** Show damage number above target */
	UFUNCTION(Client, Reliable)
	void ClientDisplayDamage(ACharacter* TargetCharacter, float DamageAmount, bool bInIsBlockedHit, bool bInIsCriticalHit);

private:

	/** Damage text's widget class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat")
	TSubclassOf<UDamageTextWidgetComponent> DamageTextWidgetClass;

#pragma endregion COMBAT

#pragma region GAS

public:

	/** Get Ability system component */
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

private:

	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

#pragma endregion GAS

#pragma region MAGIC_CIRCLE

public:

	/** Show magic circle */
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	/** Hide magic circle */
	void HideMagicCircle();

private:

	/** Update spawned magic circle's location */
	void UpdateMagicCircleLocation() const;

private:

	/** Magic circle's class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|MagicCircle")
	TSubclassOf<AMagicCircle> MagicCircleClass;

	/** Spawned magic circle */
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

#pragma endregion MAGIC_CIRCLE
	
};
