// Copyright Sergio Calahorro

#include "UI/WidgetController/OverlayWidgetController.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "GAS/Experience/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/UIWidgetRow.h"
#include "Utils/FunctionLibraries/HelperFunctionLibrary.h"

#pragma region CORE

/** Broadcast initial values */
void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());

	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

/** Bind callbacks to delegates */
void UOverlayWidgetController::BindCallbacksToDelegates()
{
	// Health callbacks
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Mana callbacks
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Give abilities
	if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
	}

	// Effect asset tag callback
	GetAuraAbilitySystemComponent()->EffectAssetTagsDelegate.AddLambda(
		[&](const FGameplayTagContainer& EffectAssetTags)
		{
			for (const FGameplayTag& Tag : EffectAssetTags)
			{
				if (!Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
				{
					continue;
				}

				if (const FUIWidgetRow* Row = UHelperFunctionLibrary::GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
				{
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);

	// Listen for changes on level and XP
	GetAuraPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPlayerState()->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnLevelChanged);
}

/** Called when player's level changes */
void UOverlayWidgetController::OnLevelChanged(int32 NewLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
}

/** Called when player's XP changes */
void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;
	check(LevelUpInfo);
	
	const int32 CurrentLevel = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (CurrentLevel > 0 && CurrentLevel <= MaxLevel)
	{
		const int32 CurrentLevelUpRequirement = LevelUpInfo->LevelUpInformation[CurrentLevel].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[CurrentLevel - 1].LevelUpRequirement;
		const int32 DeltaLevelUpRequirement = CurrentLevelUpRequirement - PreviousLevelUpRequirement;

		const int32 CurrentLevelXP = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(CurrentLevelXP) / static_cast<float>(DeltaLevelUpRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

#pragma endregion CORE