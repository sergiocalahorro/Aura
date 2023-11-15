// Copyright Sergio Calahorro

#include "GAS/AbilityTasks/AbilityTask_TargetDataUnderMouse.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "Aura.h"

#pragma region OVERRIDES

/** Called to trigger the actual task once the delegates have been set up Note that the default implementation does nothing and you don't have to call it */
void UAbilityTask_TargetDataUnderMouse::Activate()
{
	Super::Activate();

	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UAbilityTask_TargetDataUnderMouse::OnTargetDataReplicatedCallback);
		if (!AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey))
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}
	
#pragma endregion OVERRIDES

#pragma region TASK

/** Create task to retrieve target data under mouse */
UAbilityTask_TargetDataUnderMouse* UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderMouse* Task = NewAbilityTask<UAbilityTask_TargetDataUnderMouse>(OwningAbility);
	return Task;
}

/** Send data from trace under mouse */
void UAbilityTask_TargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

	const APlayerController* PlayerController = Ability->GetActorInfo().PlayerController.Get();
	if (!PlayerController)
	{
		return;
	}
	
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Target, false, CursorHit);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHit;
	TargetDataHandle.Add(TargetData);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), TargetDataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ReceivedTargetData.Broadcast(TargetDataHandle);
	}
}

/** Callback called when target data is replicated to the server */
void UAbilityTask_TargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ReceivedTargetData.Broadcast(GameplayAbilityTargetDataHandle);
	}
}
#pragma endregion TASK

