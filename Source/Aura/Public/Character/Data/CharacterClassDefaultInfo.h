// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

#include "CharacterClassDefaultInfo.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
public:
	
	/** Default values for Primary Attributes */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};
