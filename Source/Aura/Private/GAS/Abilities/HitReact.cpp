// Copyright Sergio Calahorro

#include "GAS/Abilities/HitReact.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

// Headers - Aura
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UHitReact::UHitReact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(HitReactEffect);
	
	const ICombatInterface* HitActor = CastChecked<ICombatInterface>(ActorInfo->AvatarActor.Get());
	
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	const FGameplayEffectSpecHandle EffectSpecHandle = OwnerASC->MakeOutgoingSpec(HitReactEffect, 1.f, OwnerASC->MakeEffectContext());
	ActiveHitReactEffectHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, HitActor->GetHitReactMontage());
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UHitReact::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UHitReact::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UHitReact::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UHitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(PlayMontageTask))
	{
		PlayMontageTask->EndTask();
	}
	
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	OwnerASC->RemoveActiveGameplayEffect(ActiveHitReactEffectHandle);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES