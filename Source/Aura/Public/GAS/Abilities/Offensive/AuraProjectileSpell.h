// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/Offensive/AuraRangedAttack.h"

#include "AuraProjectileSpell.generated.h"

// Forward declarations - Aura
class UAbilityTask_TargetDataUnderMouse;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraProjectileSpell : public UAuraRangedAttack
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region RANGED_ATTACK

private:

	/** Functionality performed once target data under mouse is received */
	UFUNCTION()
	void TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:

	/** Actor hit by mouse trace */
	UPROPERTY()
	TObjectPtr<AActor> MouseHitActor;

#pragma endregion RANGED_ATTACK

#pragma region ABILITY

private:

	/** Ability task used for retrieving target data under mouse */
	UPROPERTY()
	TObjectPtr<UAbilityTask_TargetDataUnderMouse> TargetDataUnderMouseTask;

#pragma endregion ABILITY
	
};
