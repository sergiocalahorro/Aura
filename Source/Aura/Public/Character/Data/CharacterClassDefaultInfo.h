// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "ScalableFloat.h"

#include "CharacterClassDefaultInfo.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
public:
	
	/** Default values for Primary Attributes */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	/** Startup abilities */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** XP reward */
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat XPReward;
};
