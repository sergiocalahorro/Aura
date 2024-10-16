// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AuraArcaneShards.generated.h"

class APointCollection;
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

protected:

	/** Class of point collection to spawn */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APointCollection> PointCollectionClass;
	
	/** Number of points */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1, UIMin = 1))
	int32 NumberOfPoints;

private:

	/** Functionality performed once the input is pressed */
	UFUNCTION()
	void InputPressed(float TimeWaited);

	/** Functionality performed once target data under mouse is received */
	UFUNCTION()
	void TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

private:

	UPROPERTY()
	TObjectPtr<APointCollection> PointCollection;

#pragma endregion ARCANE_SHARDS

#pragma region ABILITY

private:

	/** Ability task used for waiting the input to be pressed */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputPress> WaitInputPressTask;

#pragma endregion ABILITY
	
};
