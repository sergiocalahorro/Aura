// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraGameplayCueManager.h"

#pragma region OVERRIDES
	
/** Whether cues should load asynchronously at startup */
bool UAuraGameplayCueManager::ShouldAsyncLoadRuntimeObjectLibraries() const
{
	return false;
}

#pragma endregion OVERRIDES