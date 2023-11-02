// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AuraWidgetController.generated.h"

// Forward declarations - Unreal Engine
class UAttributeSet;
class UAbilitySystemComponent;

// Forward declarations - Aura
class UAbilitiesInfo;
class UAuraAbilitySystemComponent;
class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAttributeSet;
struct FAuraAbilityInfo;
struct FWidgetControllerParams;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerValueChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

#pragma region CORE

public:

	/** Set widget controller's params */
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams);

	/** Broadcast initial values */
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() PURE_VIRTUAL(UAuraWidgetController::BroadcastInitialValues);

	/** Bind callbacks to delegates */
	virtual void BindCallbacksToDelegates() PURE_VIRTUAL(UAuraWidgetController::BindCallbacksToDelegates);

	/** Getter of AuraPlayerController */
	AAuraPlayerController* GetAuraPlayerController();

	/** Getter of AuraPlayerState */
	AAuraPlayerState* GetAuraPlayerState();

	/** Getter of AuraAbilitySystemComponent */
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

	/** Getter of AuraAttributeSet */
	UAuraAttributeSet* GetAuraAttributeSet();
	
protected:

	/** Player controller */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<APlayerController> PlayerController;

	/** Player state */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<APlayerState> PlayerState;

	/** Ability system component */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Attribute set */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<UAttributeSet> AttributeSet;

	/** Aura Player controller */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	/** Aura Player state */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	/** Aura Ability system component */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/** Aura Attribute set */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

#pragma endregion CORE

#pragma region ABILITIES

protected:

	/** Broadcast ability info */
	void BroadcastAbilityInfo();

public:
	
	/** Delegate called to broadcast ability's info to widget */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;

protected:

	/** Abilities' info */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Widget|Data")
	TObjectPtr<UAbilitiesInfo> AbilitiesInfo;

#pragma endregion ABILITIES
	
};
