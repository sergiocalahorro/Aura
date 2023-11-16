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

	/** Sets default values for this object's properties */
	UAuraElectrocute();

#pragma endregion INITIALIZATION
	
};
