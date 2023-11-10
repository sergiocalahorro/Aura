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
class UBehaviorTree;

// Forward declarations - Aura
class AAuraAIController;

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

public:

	/** Called when this Pawn is possessed. Only called on the server (or in standalone) */
	virtual void PossessedBy(AController* NewController) override;
	
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

#pragma region AI

private:

	/** Set initial values for Blackboard */
	void SetInitialBlackboardValues() const;

protected:

	/** Behavior Tree */
	UPROPERTY(EditDefaultsOnly, Category = "AA|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	/** AI Controller */
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

#pragma endregion AI

#pragma region COMBAT

public:

	/** Get level */
	virtual int32 GetCurrentLevel_Implementation() override;

	/** Set new combat target */
	void SetCombatTarget_Implementation(AActor* InCombatTarget);

	/** Get current combat target */
	AActor* GetCombatTarget_Implementation() const;
	
	/** Functionality performed on death */
	virtual void Death(const FVector& DeathImpulse) override;
	
private:
	
	/** Callback called whenever HitReact's tag is changed */
	void HitReactTagChanged(const FGameplayTag GameplayTag, int32 NewCount);

private:
	
	/** Enemy level */
	UPROPERTY(EditAnywhere, Category = "AA|Combat|Core")
	int32 Level = 1;

	/** Base value for walking speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Core")
	float BaseWalkSpeed = 250.f;

	/** Time in order to destroy the enemy after it's dead */
	UPROPERTY(EditAnywhere, Category = "AA|Combat|Core")
	float LifeSpan = 5.f;

	/** Target to attack in combat */
	UPROPERTY(BlueprintReadWrite, Category = "AA|Combat|Core", meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> CombatTarget;

#pragma endregion COMBAT

#pragma region INTERACTABLE

public:

	/** Functionality performed when Actor should be highlighted (on hovered) */
	virtual void HighlightActor() override;

	/** Functionality performed when Actor should be un-highlighted (stopped being hovered) */
	virtual void UnHighlightActor() override;

#pragma endregion INTERACTABLE

#pragma region GAS

protected:

	/** Initialize ability actor info */
	virtual void InitAbilityActorInfo() override;

private:

	/** Setup health logic and listening for changes on tags */
	void SetupEnemy();

public:

	/** Delegate called whenever Health attribute is changed */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	/** Delegate called whenever MaxHealth attribute is changed */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

#pragma endregion GAS
	
};
