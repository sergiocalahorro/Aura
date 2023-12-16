// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AuraArcaneShards.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_WaitInputPress;

/**
 * 
 */
UCLASS()
class AURA_API UAuraArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
#pragma region OVERRIDES
	
protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region ARCANE_SHARDS

private:

	/** Functionality performed once the input is pressed */
	UFUNCTION()
	void InputPressed(float TimeWaited);

#pragma endregion ARCANE_SHARDS

#pragma region ABILITY

private:

	/** Ability task used for waiting the input to be pressed */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputPress> WaitInputPressTask;

#pragma endregion ABILITY
	
};
