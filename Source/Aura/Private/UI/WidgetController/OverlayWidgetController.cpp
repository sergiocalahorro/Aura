// Copyright Sergio Calahorro

#include "UI/WidgetController/OverlayWidgetController.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/AbilitySystem/Data/AbilitiesInfo.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "GAS/Experience/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/UIWidgetRow.h"
#include "Utils/FunctionLibraries/HelperFunctionLibrary.h"

#pragma region CORE

/** Broadcast initial values */
void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

/** Bind callbacks to delegates */
void UOverlayWidgetController::BindCallbacksToDelegates()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	// Health callbacks
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Mana callbacks
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	// Give abilities
	if (AuraASC->bStartupAbilitiesGiven)
	{
		OnAbilitiesGiven(AuraASC);
	}
	else
	{
		AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilitiesGiven);
	}

	// Effect asset tag callback
	AuraASC->EffectAssetTagsDelegate.AddLambda(
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
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnLevelChanged);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
}

/** Called when abilities are given */
void UOverlayWidgetController::OnAbilitiesGiven(UAuraAbilitySystemComponent* AuraASC) const
{
	if (!AuraASC->bStartupAbilitiesGiven)
	{
		return;
	}

	FBroadcastAbilitySignature BroadcastAbilityDelegate;
	BroadcastAbilityDelegate.BindLambda(
		[this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo AbilityInfo = AbilitiesInfo->FindAbilityInfoForTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
			AbilityInfo.InputTag = AuraASC->GetAbilityInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		}
	);

	AuraASC->BroadcastAbility(BroadcastAbilityDelegate);
}

/** Called when player's level changes */
void UOverlayWidgetController::OnLevelChanged(int32 NewLevel)
{
	
}

/** Called when player's XP changes */
void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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