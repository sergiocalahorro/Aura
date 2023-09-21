// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"

#pragma region CORE

/** Function called once AbilityActorInfo has been set */
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}
	
#pragma endregion CORE

#pragma region EFFECTS

/** Callback called when a Gameplay Effect is applied */
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, FString("Effect applied!"));
}

#pragma endregion EFFECTS