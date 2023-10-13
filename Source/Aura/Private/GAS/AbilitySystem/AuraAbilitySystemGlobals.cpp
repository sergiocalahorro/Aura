// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraAbilitySystemGlobals.h"

// Headers - Aura
#include "GAS/Utils/AuraAbilityTypes.h"

#pragma region OVERRIDES

/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}

#pragma endregion OVERRIDES