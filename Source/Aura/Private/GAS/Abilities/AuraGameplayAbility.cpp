// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraGameplayAbility.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraGameplayAbility::UAuraGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#pragma endregion INITIALIZATION