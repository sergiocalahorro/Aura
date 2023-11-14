// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayCueManager.h"

#include "AuraGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:

	/** Whether cues should load asynchronously at startup */
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;

#pragma endregion OVERRIDES

};
