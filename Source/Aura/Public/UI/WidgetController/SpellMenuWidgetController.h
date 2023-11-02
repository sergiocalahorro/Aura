// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraWidgetController.h"

#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

#pragma region CORE

public:

	/** Broadcast initial values */
	virtual void BroadcastInitialValues() override;

	/** Bind callbacks to delegates */
	virtual void BindCallbacksToDelegates() override;

#pragma endregion CORE
};
