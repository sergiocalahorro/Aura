// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraDamageGameplayAbility.h"
#include "Interaction/TaggedMontage.h"

#include "AuraMeleeAttack.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class AURA_API UAuraMeleeAttack : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region MELEE_ATTACK

private:

	/** Perform Melee Attack */
	UFUNCTION()
	void MeleeAttack(FGameplayEventData Payload);

private:
	
	/** Attack's radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|MeleeAttack", meta = (ClampMin = 0.f, UIMin = 0.f, Delta = 0.1f))
	float AttackRadius = 45.f;

	/** Attack's montage and tag */
	FTaggedMontage TaggedAttackMontage;

#pragma endregion MELEE_ATTACK

#pragma region ABILITY

private:

	/** Ability task used for playing the Montage for performing the melee attack */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	/** Ability task used for waiting for a GameplayTag event */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;

#pragma endregion ABILITY
};
