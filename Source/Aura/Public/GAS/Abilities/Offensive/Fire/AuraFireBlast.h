// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

#pragma region FIREBOLT

protected:

	/** Number of fire balls to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "AA|FireBlast", meta = (ClampMin = 1, UIMin = 1))
	int32 NumberOfFireBalls = 12;

#pragma endregion FIREBOLT

#pragma region DESCRIPTION
	
public:

	/** Get ability's description */
	virtual FString GetDescription(int32 Level) override;

	/** Get ability's description for next level */
	virtual FString GetNextLevelDescription(int32 Level) override;

#pragma endregion DESCRIPTION
};
