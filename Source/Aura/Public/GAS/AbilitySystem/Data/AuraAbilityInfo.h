// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "AuraAbilityInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()
	
public:

	/** Ability's GameplayTag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;

	/** Ability's input GameplayTag */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;

	/** Ability's icon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon;

	/** Ability's background material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial;
};
