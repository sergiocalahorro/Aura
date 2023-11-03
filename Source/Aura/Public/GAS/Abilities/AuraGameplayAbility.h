// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

// Headers - Aura
#include "Interaction/AttackData.h"

#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UAuraGameplayAbility();

#pragma endregion INITIALIZATION

#pragma region INPUT
	
public:

	/** Input tag this ability has at startup */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	FGameplayTag StartupInputTag;

#pragma endregion INPUT

#pragma region DESCRIPTION
	
public:

	/** Get ability's description */
	virtual FString GetDescription(int32 Level);

	/** Get ability's description for next level */
	virtual FString GetNextLevelDescription(int32 Level);

	/** Get ability's description when it's locked */
	static FString GetLockedDescription(int32 Level);

#pragma endregion DESCRIPTION
	
};
