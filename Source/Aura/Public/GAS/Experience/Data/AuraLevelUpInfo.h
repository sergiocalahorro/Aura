// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

#include "AuraLevelUpInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()
	
public:

	/** Experience points required to level up */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0, UIMin = 0))
	int32 LevelUpRequirement = 0;

	/** Attribute points rewarded on level up */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1, UIMin = 1, ClampMax = 5, UIMax = 5))
	int32 AttributePointReward = 1;

	/** Spell points rewarded on level up */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1, UIMin = 1, ClampMax = 5, UIMax = 5))
	int32 SpellPointReward = 1;
};
