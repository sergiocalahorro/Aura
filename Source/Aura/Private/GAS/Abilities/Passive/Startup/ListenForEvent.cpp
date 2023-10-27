// Copyright Sergio Calahorro

#include "GAS/Abilities/Passive/Startup/ListenForEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UListenForEvent::UListenForEvent()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UListenForEvent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag, nullptr, false, false);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UListenForEvent::OnEventReceived);
	WaitEventTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UListenForEvent::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WaitEventTask))
	{
		WaitEventTask->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES

#pragma region EVENT

/** Function called whenever an event with the given tag is received */
void UListenForEvent::OnEventReceived(FGameplayEventData Payload)
{
	UAbilitySystemComponent* OwningASC = GetAbilitySystemComponentFromActorInfo();
	const FGameplayEffectContextHandle EffectContextHandle = OwningASC->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle = OwningASC->MakeOutgoingSpec(EventBasedEffectClass, 1.f, EffectContextHandle);
	const FGameplayEffectSpecHandle SetByCallerEffectSpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Payload.EventTag, Payload.EventMagnitude);
	OwningASC->ApplyGameplayEffectSpecToSelf(*SetByCallerEffectSpecHandle.Data.Get());
}

#pragma endregion EVENT