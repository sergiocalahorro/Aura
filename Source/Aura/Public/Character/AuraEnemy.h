// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraBaseCharacter.h"
#include "Interaction/InteractableInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraEnemy.generated.h"

// Forward declarations - Unreal Engine
class UWidgetComponent;

UCLASS(Abstract)
class AURA_API AAuraEnemy : public AAuraBaseCharacter, public IInteractableInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this character's properties */
	AAuraEnemy();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

private:

	/** Health bar's widget component */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

#pragma endregion COMPONENTS

#pragma region CORE

private:
	
	/** Enemy level */
	UPROPERTY(EditAnywhere, Category = "AA|Core")
	int32 Level = 1;

#pragma endregion CORE

#pragma region COMBAT

public:

	/** Get level */
	virtual int32 GetCurrentLevel() const override; 

#pragma endregion COMBAT

#pragma region INTERACTABLE

public:

	/** Functionality performed when Actor should be highlighted (on hovered) */
	virtual void HighlightActor() override;

	/** Functionality performed when Actor should be un-highlighted (stopped being hovered) */
	virtual void UnHighlightActor() override;

#pragma endregion INTERACTABLE

#pragma region GAS

public:

	/** Delegate called whenever Health attribute is changed */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	/** Delegate called whenever MaxHealth attribute is changed */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:

	/** Initialize ability actor info */
	virtual void InitAbilityActorInfo() override;

private:

	/** Setup health bar's widget controller and delegates for broadcasting health values */
	void SetupHealthBindings();

#pragma endregion GAS
	
};
