// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

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
	
protected:

	/** Callback called when a Gameplay Effect is applied */
	UFUNCTION()
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);

#pragma endregion EFFECTS

};
