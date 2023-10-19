// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
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

#pragma region ATTACK

protected:
	
	/** Get attack to use */
	virtual FAttackData GetAttackToUse(const TArray<FAttackData>& Attacks, const FGameplayTag& AttackTag = FGameplayTag());

protected:
	
	/** Current attack data to use in this ability */
	FAttackData CurrentAttackData;

#pragma endregion ATTACK
	
};
