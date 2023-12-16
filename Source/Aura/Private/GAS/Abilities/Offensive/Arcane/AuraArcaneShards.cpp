// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Arcane/AuraArcaneShards.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

// Headers - Aura
#include "Interaction/PlayerInterface.h"

#pragma region OVERRIDES
	
/** Actually activate ability, do not call this directly */
void UAuraArcaneShards::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_ShowMagicCircle(GetAvatarActorFromActorInfo(), nullptr);
	}

	WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPressTask->OnPress.AddUniqueDynamic(this, &UAuraArcaneShards::InputPressed);
	WaitInputPressTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraArcaneShards::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WaitInputPressTask))
	{
		WaitInputPressTask->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES

#pragma region ARCANE_SHARDS

/** Functionality performed once the input is pressed */
void UAuraArcaneShards::InputPressed(float TimeWaited)
{
	if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_HideMagicCircle(GetAvatarActorFromActorInfo());
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

#pragma endregion ARCANE_SHARDS