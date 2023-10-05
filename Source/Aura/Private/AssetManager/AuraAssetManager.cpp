﻿// Copyright Sergio Calahorro

#include "AssetManager/AuraAssetManager.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"

#pragma region INITIALIZATION

/** Get instance */
UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Starts initial load, gets called from InitializeObjectReferences */
void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FAuraGameplayTags::InitializeNativeGameplayTags();
}

#pragma endregion OVERRIDES