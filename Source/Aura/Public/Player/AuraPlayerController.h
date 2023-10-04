// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "AuraPlayerController.generated.h"

// Forward declarations - Unreal Engine
class UInputMappingContext;
class UInputAction;
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
