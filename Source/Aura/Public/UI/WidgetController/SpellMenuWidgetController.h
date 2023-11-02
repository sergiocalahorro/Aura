// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Headers - Aura
#include "AuraWidgetController.h"

#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bCanSpendPoints, bool, bCanEquipSpell);

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

	/** Function called when a spell is selected */
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:

	/** Update selected spell information */
	void UpdateSelectedSpellInfo(const FGameplayTag& StatusTag, int32 SpellPoints) const;

	/** Check spell status in order to enable/disable the options to spend points and equip the spell */
	static void CheckSpellStatus(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bCanSpendPoints, bool& bCanEquipSpell);
	
public:

	/** Delegate called when spell points change */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerValueChangedSignature SpellPointsChangedDelegate;

	/** Delegate called to broadcast information up to the spell menu about the options there are once a spell is selected */
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

private:

	/** Currently selected spell's info */
	FSelectedAbility SelectedSpell;

	/** Current spell points */
	int32 CurrentSpellPoints;
	
#pragma endregion SPELLS

};
