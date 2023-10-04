// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	/** Input tag this ability has at startup */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	FGameplayTag StartupInputTag;
};
