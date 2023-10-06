// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AbilityTask_TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UAbilityTask_TargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:
	
	/** Called to trigger the actual task once the delegates have been set up Note that the default implementation does nothing and you don't have to call it */
	virtual void Activate() override;
	
#pragma endregion OVERRIDES

#pragma region TASK

public:

	/** Create task to retrieve target data under mouse */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_TargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

private:

	/** Send data from trace under mouse */
	void SendMouseCursorData();

	/** Callback called when target data is replicated to the server */
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);
	
public:

	/** Delegate called to broadcast target data under mouse */
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ReceivedTargetData;

#pragma endregion TASK

};
