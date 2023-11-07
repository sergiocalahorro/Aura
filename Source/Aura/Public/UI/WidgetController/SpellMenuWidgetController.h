// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Headers - Aura
#include "AuraWidgetController.h"

#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bCanSpendPoints, bool, bCanEquipSpell, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityTypeTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
public:

	/** Ability's tag */
	FGameplayTag AbilityTag;

	/** Ability's status tag */
	FGameplayTag StatusTag;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	USpellMenuWidgetController();

#pragma endregion INITIALIZATION

#pragma region CORE

public:

	/** Broadcast initial values */
	virtual void BroadcastInitialValues() override;

	/** Bind callbacks to delegates */
	virtual void BindCallbacksToDelegates() override;

#pragma endregion CORE

#pragma region SPELLS

public:

	/** Setup information about the selected spell */
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	/** Clear information about the selected spell */
	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeselected();

	/** Spend spell point */
	UFUNCTION(BlueprintCallable)
	void SpendSpellPoint();

	/** Equip spell */
	UFUNCTION(BlueprintCallable)
	void EquipSpell();

	/** Handle spell selected from spell row */
	UFUNCTION(BlueprintCallable)
	void HandleSpellRowGlobe(const FGameplayTag& InputTag, const FGameplayTag& TypeTag);

private:

	/** Update selected spell information */
	void UpdateSelectedSpellInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 SpellPoints);

	/** Check spell status in order to enable/disable the options to spend points and equip the spell */
	static void CheckSpellStatus(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bOutCanSpendPoints, bool& bOutCanEquipSpell);

	/** Callback called when ability is equipped */
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);
	
public:

	/** Delegate called when spell points change */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerValueChangedSignature SpellPointsChangedDelegate;

	/** Delegate called to broadcast information up to the spell menu about the options there are once a spell is selected */
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	/** Delegate called to broadcast information up to the spell menu to wait for equipping a spell */
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;

	/** Delegate called to broadcast information up to the spell menu to stop the wait for equipping a spell */
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipSelectionDelegate;

	/** Delegate called when a spell is reassigned to a different slot */
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;

private:

	/** Currently selected ability's info */
	FSelectedAbility SelectedAbility;

	/** Current spell points */
	int32 CurrentSpellPoints = 0;

	/** Whether it's in waiting state for equipping a spell */
	bool bWaitingForEquipSelection = false;
	
	/** Tag belonging to the selected input slot */
	FGameplayTag SelectedSlotTag;
	
#pragma endregion SPELLS

};
