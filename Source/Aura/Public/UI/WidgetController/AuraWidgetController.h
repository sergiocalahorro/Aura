﻿// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AuraWidgetController.generated.h"

// Forward declarations - Unreal Engine
class UAttributeSet;
class UAbilitySystemComponent;

// Forward declarations - Aura
struct FWidgetControllerParams;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerValueChangedSignature, int32, NewValue);

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

#pragma endregion CORE
	
};
