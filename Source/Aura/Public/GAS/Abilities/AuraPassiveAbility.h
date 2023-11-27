// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraGameplayAbility.h"

#include "AuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UAuraPassiveAbility();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region PASSIVE

private:

	/** Callback called when the passive ability bound to this tag should be deactivated */
	void OnDeactivatePassiveAbility(const FGameplayTag& AbilityTag);

protected:

	/** Passive ability's gameplay cue tag */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Passive")
	FGameplayTag PassiveCueTag;

private:

	/** Passive's gameplay cue parameters */
	FGameplayCueParameters PassiveCueParams;

#pragma endregion PASSIVE
	
};
