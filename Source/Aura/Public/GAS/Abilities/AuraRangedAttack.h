// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraDamageGameplayAbility.h"

#include "AuraRangedAttack.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

// Forward declarations - Aura
class AAuraProjectile;
class ICombatInterface;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraRangedAttack : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UAuraRangedAttack();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region RANGED_ATTACK

protected:

	/** Spawn projectile */
	UFUNCTION()
	virtual void SpawnProjectile(FGameplayEventData Payload);

protected:

	/** Event's gameplay tag to wait for to spawn the projectile (if no tag is set, a random attack will be used) */
	UPROPERTY(EditDefaultsOnly, Category = "AA|RangedAttack")
	FGameplayTag SpawnProjectileEventTag;
	
	/** Location the projectile should be aimed at */
	UPROPERTY()
	FVector ProjectileTargetLocation;

#pragma endregion RANGED_ATTACK

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
