// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

// Headers - Aura
#include "Interaction/CombatInterface.h"

#include "AuraBaseCharacter.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

// Forward declarations - Aura
struct FEffectDefinition;
struct FActiveGameplayEffectHandle;

UCLASS(Abstract)
class AURA_API AAuraBaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
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
	
	/** Apply given effect to itself */
	FActiveGameplayEffectHandle ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float Level) const;

	/** Apply given effect definitions to itself */
	void ApplyEffectDefinitionsToSelf(const TArray<FEffectDefinition>& Effects) const;

	/** Grant abilities to the character */
	virtual void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

protected:

	/** Default effects to apply when ability system is initialized
	 *  NOTE: When setting effects to initialize attributes, make sure Primary attributes are set BEFORE Secondary ones,
	 *		  as they depend on them */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<FEffectDefinition> DefaultEffects;

	/** Abilities granted at startup */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/** Attribute set */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

#pragma endregion GAS
	
};
