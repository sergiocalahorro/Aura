// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "WaitCooldownChange.generated.h"


// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

#pragma region COOLDOWN

public:
	
	/** Create async task for waiting for changes on the cooldown */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = true))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayTag& InCooldownTag);

	/** End async task */
	UFUNCTION(BlueprintCallable)
	void EndTask();

private:
	
	/** Function called whenever an effect is applied */
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);

	/** Function called whenever there's a change on InCooldownTag */
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	
public:

	/** Delegate called when cooldown starts */
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStartDelegate;

	/** Delegate called when cooldown ends */
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEndDelegate;

protected:

	/** Ability system component that the cooldown is applied to */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Cooldown's tag */
	UPROPERTY()
	FGameplayTag CooldownTag;

#pragma endregion COOLDOWN

};
