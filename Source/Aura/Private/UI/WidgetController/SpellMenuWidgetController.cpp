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
	CurrentSpellPoints = 0;
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
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			// Handle the case when there is an already selected spell, in order to properly update the options on the UI
			if (SelectedSpell.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedSpell.StatusTag = StatusTag;
				UpdateSelectedSpellInfo(StatusTag, CurrentSpellPoints);
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
			UpdateSelectedSpellInfo(SelectedSpell.StatusTag, CurrentSpellPoints);
		}
	);
}

#pragma endregion CORE

#pragma region SPELLS

/** Function called when a spell is selected */
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
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
	UpdateSelectedSpellInfo(StatusTag, SpellPoints);
}

/** Update selected spell information */
void USpellMenuWidgetController::UpdateSelectedSpellInfo(const FGameplayTag& StatusTag, int32 SpellPoints) const
{
	bool bCanSpendPoints = false;
	bool bCanEquipSpell = false;
	CheckSpellStatus(StatusTag, SpellPoints, bCanSpendPoints, bCanEquipSpell);
	SpellGlobeSelectedDelegate.Broadcast(bCanSpendPoints, bCanEquipSpell);
}

/** Check spell status in order to enable/disable the options to spend points and equip the spell */
void USpellMenuWidgetController::CheckSpellStatus(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bCanSpendPoints, bool& bCanEquipSpell)
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
	{
		bCanEquipSpell = true;
		bCanSpendPoints = SpellPoints > 0;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
	{
		bCanEquipSpell = false;
		bCanSpendPoints = SpellPoints > 0;
	}
	if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
	{
		bCanEquipSpell = true;
		bCanSpendPoints = SpellPoints > 0;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Locked))
	{
		bCanEquipSpell = false;
		bCanSpendPoints = false;
	}
}

#pragma endregion SPELLS
