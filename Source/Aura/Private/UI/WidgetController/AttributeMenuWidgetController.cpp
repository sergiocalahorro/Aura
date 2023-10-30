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
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AuraAttributeSet->TaggedAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnAttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
}

/** Bind callbacks to delegates */
void UAttributeMenuWidgetController::BindCallbacksToDelegates()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AuraAttributeSet->TaggedAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnAttributePointsChangedDelegate.Broadcast(NewValue);
		}
	);
}

/** Upgrade attribute with given tag */
void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

/** Broadcast attribute info */
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo AttributeInfo = AttributesInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}

#pragma endregion CORE