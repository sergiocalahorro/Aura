// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Headers - Aura
#include "GAS/AbilitySystem/Data/AuraAbilityInfo.h"

#include "AbilitiesInfo.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAbilitiesInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Find Ability's info by GameplayTag */
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;

public:

	/** Abilities' information */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAbilityInfo> AbilitiesInformation;
};
