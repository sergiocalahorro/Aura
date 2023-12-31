﻿// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraDamageGameplayAbility.h"

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

	/** Event received for spawning projectile */
	UFUNCTION()
	virtual void EventReceivedSpawnProjectile(FGameplayEventData Payload);

	/** Spawn projectile */
	void SpawnProjectile(TSubclassOf<AAuraProjectile> ProjectileClass, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bInOverridePitch = false, float InPitchOverride = 0.f, AActor* HomingTarget = nullptr) const;

protected:

	/** Maximum number of projectiles launched */
	UPROPERTY(EditDefaultsOnly, Category = "AA|RangedAttack", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxNumberOfProjectiles = 5;

	/** Whether projectile's spawn pitch rotation should be overridden */
	UPROPERTY(EditDefaultsOnly, Category = "AA|RangedAttack")
	bool bOverridePitch = true;

	/** Projectile's spawn pitch rotation */
	UPROPERTY(EditDefaultsOnly, Category = "AA|RangedAttack", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 360.f, UIMax = 360.f, Delta = 1.f, Units = "Degrees"))
	float PitchOverride = 45.f;
	
	/** Location the projectile should be aimed at */
	UPROPERTY()
	FVector ProjectileTargetLocation;

#pragma endregion RANGED_ATTACK

#pragma region ABILITY

private:

	/** Ability task used for playing the Montage for performing the ranged attack */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	/** Ability task used for waiting for a GameplayTag event */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;
	
#pragma endregion ABILITY

};
