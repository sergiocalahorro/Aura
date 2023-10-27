// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Headers - Aura
#include "GAS/Experience/Data/AuraLevelUpInfo.h"

#include "LevelUpInfo.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Find level for incoming XP */
	int32 FindLevelForXP(int32 XP) const;

public:

	/** Information for each level */
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;
};
