// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "AuraBaseCharacter.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this character's properties */
	AAuraBaseCharacter();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

protected:

	/** Weapon mesh */
	UPROPERTY(EditAnywhere, Category = "AA|Components")
	TObjectPtr<USkeletalMeshComponent> Weapon;

#pragma endregion COMPONENTS

#pragma region GAS

public:

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the attribute set system component to use for this actor */
	UAttributeSet* GetAttributeSet() const;

protected:

	/** Initialize ability actor info */
	virtual void InitAbilityActorInfo() PURE_VIRTUAL(AAuraBaseCharacter::InitAbilityActorInfo);

protected:
	
	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/** Attribute set */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

#pragma endregion GAS
	
};
