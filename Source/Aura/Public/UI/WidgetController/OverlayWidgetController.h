// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "UI/Widget/UIWidgetRow.h"
#include "UI/WidgetController/AuraWidgetController.h"

#include "OverlayWidgetController.generated.h"

// Forward declarations - Unreal Engine
class UDataTable;
struct FOnAttributeChangeData;

// Forward declarations - Aura
class UAbilitiesInfo;
class UAuraAbilitySystemComponent;
struct FAuraAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, const FUIWidgetRow&, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
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

	/** Called when abilities are given */
	void OnAbilitiesGiven(UAuraAbilitySystemComponent* AuraASC) const;

public:

	/** Delegate called when Health Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	/** Delegate called when MaxHealth Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	/** Delegate called when Mana Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	/** Delegate called when MaxMana Attribute is changed */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	/** Delegate called when an effect is applied, to send data to the Widget associated with the effect's message tag */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Attributes")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	/** Delegate called to broadcast ability's info to widget */
	UPROPERTY(BlueprintAssignable, Category = "AA|GAS|Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;

protected:

	/** Message widget data table */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Widget|Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/** Abilities' info */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Widget|Data")
	TObjectPtr<UAbilitiesInfo> AbilitiesInfo;

#pragma endregion CORE
	
};
