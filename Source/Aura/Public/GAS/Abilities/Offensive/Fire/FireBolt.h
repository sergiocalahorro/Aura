// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraProjectileSpell.h"

#include "FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

#pragma region DESCRIPTION
	
public:

	/** Get ability's description */
	virtual FString GetDescription(int32 Level) override;

	/** Get ability's description for next level */
	virtual FString GetNextLevelDescription(int32 Level) override;

#pragma endregion DESCRIPTION
};
