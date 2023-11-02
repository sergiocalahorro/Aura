// Copyright Sergio Calahorro

#include "UI/WidgetController/AttributeMenuWidgetController.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "GAS/Attributes/Data/AttributesInfo.h"
#include "GAS/Attributes/Data/AuraAttributeInfo.h"
#include "Player/AuraPlayerState.h"

#pragma region CORE

/** Broadcast initial values */
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributesInfo);
	for (auto& Pair : GetAuraAttributeSet()->TaggedAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	OnAttributePointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoints());
}

/** Bind callbacks to delegates */
void UAttributeMenuWidgetController::BindCallbacksToDelegates()
{
	for (auto& Pair : GetAuraAttributeSet()->TaggedAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnAttributePointsChangedDelegate.Broadcast(NewValue);
		}
	);
}

/** Upgrade attribute with given tag */
void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
}

/** Broadcast attribute info */
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo AttributeInfo = AttributesInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}

#pragma endregion CORE