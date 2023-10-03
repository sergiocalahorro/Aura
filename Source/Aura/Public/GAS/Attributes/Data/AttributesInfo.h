// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "AttributesInfo.generated.h"

// Forward declarations - Aura
struct FAuraAttributeInfo;

/**
 * 
 */
UCLASS()
class AURA_API UAttributesInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Find Attribute's info by GameplayTag */
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

public:
	
	/** Attributes' information */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributesInformation;
};
