// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "AuraAbilityInfo.generated.h"

// Forward declarations - Unreal Engine
class UGameplayAbility;

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

	/** Ability's type GameplayTag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TypeTag;

	/** Ability's input GameplayTag */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;

	/** Ability's status GameplayTag */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag;
	
	/** Ability's cooldown GameplayTag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag;

	/** Level required in order to unlock the ability */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, UIMin = 1))
	int32 LevelRequirement = 1;

	/** Ability class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	/** Ability's icon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon;

	/** Ability's background material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial;
};
