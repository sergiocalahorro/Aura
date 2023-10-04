// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer&);

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region CORE

public:

	/** Function called once AbilityActorInfo has been set */
	void AbilityActorInfoSet();
	
#pragma endregion CORE

#pragma region EFFECTS

public:

	/** Delegate called when an effect is applied, to broadcast its asset tags */
	FEffectAssetTagsSignature EffectAssetTagsDelegate;
	
protected:

	/** Callback called when a Gameplay Effect is applied */
	UFUNCTION()
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);

#pragma endregion EFFECTS

#pragma region ABILITIES

public:

	/** Add given list of abilities */
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	/** Activate ability by input tag when released */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/** Activate ability by input tag when held */
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

#pragma endregion ABILITIES

};
