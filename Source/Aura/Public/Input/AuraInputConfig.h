// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "AuraInputConfig.generated.h"

// Forward declarations - Aura
class UInputAction;

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Find an ability's input action by its GameplayTag */
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

public:

	/** Abilities' input actions */
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TObjectPtr<UInputAction>> AbilityInputActions;
};
