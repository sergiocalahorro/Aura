// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "UI/WidgetController/AuraWidgetController.h"

#include "OverlayWidgetController.generated.h"

// Forward declarations - Unreal Engine
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

#pragma region CORE
	
public:

	/** Broadcast initial values */
	virtual void BroadcastInitialValues() override;

	/** Bind callbacks to delegates */
	virtual void BindCallbacksToDelegates() override;

protected:

	/** Callback called when Health attribute is modified */
	void HealthChanged(const FOnAttributeChangeData& Data) const;

	/** Callback called when MaxHealth attribute is modified */
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	/** Callback called when Mana attribute is modified */
	void ManaChanged(const FOnAttributeChangeData& Data) const;

	/** Callback called when MaxMana attribute is modified */
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;

public:

	/** Delegate called when Health Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	/** Delegate called when MaxHealth Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	/** Delegate called when Mana Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	/** Delegate called when MaxMana Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

#pragma endregion CORE
	
};
