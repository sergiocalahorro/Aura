// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "ListenForEvent.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UListenForEvent : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UListenForEvent();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region EVENT

private:

	/** Function called whenever an event with the given tag is received */
	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);

private:

	/** Ability task used for waiting for a GameplayTag event */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Event")
	FGameplayTag EventTag;

	/** Effect to apply once the event is received */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Event")
	TSubclassOf<UGameplayEffect> EventBasedEffectClass;

	/** Ability task used for waiting for a GameplayTag event */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;
	
#pragma endregion EVENT
	
};
