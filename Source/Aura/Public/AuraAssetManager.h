// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:

	/** Get instance */
	static UAuraAssetManager& Get();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;

#pragma endregion OVERRIDES
};
