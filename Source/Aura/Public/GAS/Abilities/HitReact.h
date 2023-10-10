// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraGameplayAbility.h"

#include "HitReact.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UHitReact : public UAuraGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UHitReact();
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region HIT_REACT

private:

	/** HitReact's effect to apply */
	UPROPERTY(EditDefaultsOnly, Category = "AA|HitReact")
	TSubclassOf<UGameplayEffect> HitReactEffect;

	/** Ability task used for playing the Montage for hit reacting */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	/** Handle used for removing the hit reacting effect */
	UPROPERTY()
	FActiveGameplayEffectHandle ActiveHitReactEffectHandle;

#pragma endregion HIT_REACT

};
