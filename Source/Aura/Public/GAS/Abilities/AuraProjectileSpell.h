// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"

#include "AuraProjectileSpell.generated.h"

// Forward declarations - Aura
class AAuraProjectile;

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

	/** Projectile to spawn's class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TSubclassOf<AAuraProjectile> ProjectileClass;

#pragma endregion PROJECTILE
	
};
