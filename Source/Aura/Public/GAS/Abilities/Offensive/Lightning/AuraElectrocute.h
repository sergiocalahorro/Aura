// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/Offensive/AuraBeamSpell.h"

#include "AuraElectrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UAuraElectrocute();

#pragma endregion INITIALIZATION

#pragma region DESCRIPTION

public:
	
	/** Get ability's description */
	virtual FString GetDescription(int32 Level) override;

	/** Get ability's description for next level */
	virtual FString GetNextLevelDescription(int32 Level) override;

#pragma endregion DESCRIPTION
	
};
