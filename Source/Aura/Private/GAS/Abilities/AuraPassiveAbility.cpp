// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraPassiveAbility.h"

// Headers - Unreal Engine
#include "GameplayCueFunctionLibrary.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraPassiveAbility::UAuraPassiveAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
/** Actually activate ability, do not call this directly */
void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UGameplayCueFunctionLibrary::AddGameplayCueOnActor(GetAvatarActorFromActorInfo(), PassiveCueTag, PassiveCueParams);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		AuraASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &UAuraPassiveAbility::OnDeactivatePassiveAbility);
	}
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraPassiveAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(GetAvatarActorFromActorInfo(), PassiveCueTag, PassiveCueParams);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES

#pragma region PASSIVE

/** Callback called when the passive ability bound to this tag should be deactivated */
void UAuraPassiveAbility::OnDeactivatePassiveAbility(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

#pragma endregion PASSIVE