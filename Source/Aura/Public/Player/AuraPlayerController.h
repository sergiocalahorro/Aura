// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "AuraPlayerController.generated.h"

// Forward declarations - Unreal Engine
class UInputMappingContext;
class UInputAction;
class USplineComponent;
struct FInputActionValue;
struct FGameplayTag;

// Forward declarations - Aura
class IInteractableInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;

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
	
	/** Called on the client to do local pawn setup after possession, before calling ServerAcknowledgePossession */
	virtual void AcknowledgePossession(APawn* PossessedPawn) override;

	/** Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation() */
	virtual void PlayerTick(float DeltaTime) override;

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings */
	virtual void SetupInputComponent() override;

#pragma endregion OVERRIDES

#pragma region CORE

private:

	/** Controlled pawn */
	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;

#pragma endregion CORE

#pragma region INPUT

private:

	/** Move player */
	void Move(const FInputActionValue& InputActionValue);

	/** Callback for Input Pressed */
	void AbilityInputTagPressed(FGameplayTag InputTag);

	/** Callback for Input Released */
	void AbilityInputTagReleased(FGameplayTag InputTag);

	/** Callback for Input Held */
	void AbilityInputTagHeld(FGameplayTag InputTag);

private:

	/** Input Mapping Context */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputMappingContext> AuraMappingContext;

	/** Input Action for Movement */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> MoveInputAction;

	/** Input Config */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

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

#pragma endregion MOVEMENT

#pragma region INTERACTABLE

private:
	
	/** Trace hit under cursor to highlight Actors */
	void CursorTrace(float DeltaTime);

private:

	/** Last Actor that was highlighted */
	IInteractableInterface* LastHighlightedActor;

	/** Current Actor that is highlighted */
	IInteractableInterface* CurrentHighlightedActor;

#pragma endregion INTERACTABLE

#pragma region GAS

public:

	/** Get Ability system component */
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

private:

	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

#pragma endregion GAS
	
};
