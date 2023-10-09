// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"

#include "AuraProjectileSpell.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

// Forward declarations - Aura
class AAuraProjectile;
class ICombatInterface;
class UAbilityTask_TargetDataUnderMouse;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region PROJECTILE

private:

	/** Cast projectile and launch it */
	UFUNCTION()
	void SpawnProjectile(FGameplayEventData Payload);

	/** Functionality performed once target data under mouse is received */
	UFUNCTION()
	void TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

private:

	/** Projectile to spawn's class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TSubclassOf<AAuraProjectile> ProjectileClass;

	/** Class for the damage effect to apply */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Montage played to cast projectile */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<UAnimMontage> CastProjectileMontage;

	/** Event's gameplay tag to wait for to spawn the projectile */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	FGameplayTag CastProjectileEventTag;

#pragma endregion PROJECTILE

#pragma region ABILITY

private:

	/** Ability task used for playing the Montage for casting the projectile */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;
	
	/** Ability task used for waiting for a GameplayTag event */
    UPROPERTY()
    TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;

	/** Ability task used for retrieving target data under mouse */
	UPROPERTY()
	TObjectPtr<UAbilityTask_TargetDataUnderMouse> TargetDataUnderMouseTask;

	/** Projectile's caster */
	ICombatInterface* ProjectileCaster;

	/** Location the projectile should be aimed at */
	UPROPERTY()
	FVector ProjectileTargetLocation;

#pragma endregion ABILITY
	
};
