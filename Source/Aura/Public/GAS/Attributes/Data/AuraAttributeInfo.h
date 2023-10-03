// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "AuraAttributeInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()
	
public:

	/** Attribute's GameplayTag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	/** Attribute's name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName;

	/** Attribute's description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription;

	/** Attribute's value */
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue;
};
