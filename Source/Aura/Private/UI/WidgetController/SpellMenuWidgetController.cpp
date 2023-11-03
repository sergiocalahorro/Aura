// Copyright Sergio Calahorro

#include "UI/WidgetController/SpellMenuWidgetController.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/AbilitySystem/Data/AbilitiesInfo.h"
#include "Player/AuraPlayerState.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
USpellMenuWidgetController::USpellMenuWidgetController()
{
	SelectedSpell = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
}

#pragma endregion INITIALIZATION

#pragma region CORE

/** Broadcast initial values */
void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

/** Bind callbacks to delegates */
void USpellMenuWidgetController::BindCallbacksToDelegates()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewAbilityLevel)
		{
			// Handle the case when there is an already selected spell, in order to properly update the options on the UI
			if (SelectedSpell.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedSpell.StatusTag = StatusTag;
				UpdateSelectedSpellInfo(AbilityTag, StatusTag, CurrentSpellPoints);
			}
			
			if (AbilitiesInfo)
			{
				FAuraAbilityInfo AbilityInfo = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag);
				AbilityInfo.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(AbilityInfo);
			}
		}
	);

	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;

			// Handle the case when there is an already selected spell, in order to properly update the options on the UI
			UpdateSelectedSpellInfo(SelectedSpell.AbilityTag, SelectedSpell.StatusTag, CurrentSpellPoints);
		}
	);
}

#pragma endregion CORE

#pragma region SPELLS

/** Setup information about the selected spell */
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag AbilityTypeTag = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag).TypeTag;
		StopWaitForEquipSelectionDelegate.Broadcast(AbilityTypeTag);
		bWaitingForEquipSelection = false;
	}
	
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag StatusTag;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(AuraGameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);

	if (!bTagValid || bTagNone || !AbilitySpec)
	{
		StatusTag = AuraGameplayTags.Abilities_Status_Locked;
	}
	else
	{
		StatusTag = GetAuraAbilitySystemComponent()->GetAbilityStatusTagFromSpec(*AbilitySpec);
	}

	SelectedSpell.AbilityTag = AbilityTag;
	SelectedSpell.StatusTag = StatusTag;
	UpdateSelectedSpellInfo(AbilityTag, StatusTag, SpellPoints);
}

/** Clear information about the selected spell */
void USpellMenuWidgetController::SpellGlobeDeselected()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag AbilityTypeTag = AbilitiesInfo->FindAbilityInfoForTag(SelectedSpell.AbilityTag).TypeTag;
		StopWaitForEquipSelectionDelegate.Broadcast(AbilityTypeTag);
		bWaitingForEquipSelection = false;
	}
	
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	SelectedSpell.AbilityTag = AuraGameplayTags.Abilities_None;
	SelectedSpell.StatusTag = AuraGameplayTags.Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

/** Spend spell point */
void USpellMenuWidgetController::SpendSpellPoint()
{
	GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(SelectedSpell.AbilityTag);
}

/** Equip spell */
void USpellMenuWidgetController::EquipSpell()
{
	const FGameplayTag AbilityTypeTag = AbilitiesInfo->FindAbilityInfoForTag(SelectedSpell.AbilityTag).TypeTag;
	WaitForEquipSelectionDelegate.Broadcast(AbilityTypeTag);
	bWaitingForEquipSelection = true;
}

/** Update selected spell information */
void USpellMenuWidgetController::UpdateSelectedSpellInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 SpellPoints)
{
	bool bCanSpendPoints = false;
	bool bCanEquipSpell = false;
	FString Description;
	FString NextLevelDescription;
	CheckSpellStatus(StatusTag, SpellPoints, bCanSpendPoints, bCanEquipSpell);
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bCanSpendPoints, bCanEquipSpell, Description, NextLevelDescription);
}

/** Check spell status in order to enable/disable the options to spend points and equip the spell */
void USpellMenuWidgetController::CheckSpellStatus(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bOutCanSpendPoints, bool& bOutCanEquipSpell)
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
	{
		bOutCanEquipSpell = true;
		bOutCanSpendPoints = SpellPoints > 0;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
	{
		bOutCanEquipSpell = false;
		bOutCanSpendPoints = SpellPoints > 0;
	}
	if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
	{
		bOutCanEquipSpell = true;
		bOutCanSpendPoints = SpellPoints > 0;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Locked))
	{
		bOutCanEquipSpell = false;
		bOutCanSpendPoints = false;
	}
}

#pragma endregion SPELLS
