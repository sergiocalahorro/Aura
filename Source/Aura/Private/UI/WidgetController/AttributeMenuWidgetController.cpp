// Copyright Sergio Calahorro

#include "UI/WidgetController/AttributeMenuWidgetController.h"

// Headers - Aura
#include "GAS/Attributes/AuraAttributeSet.h"
#include "GAS/Attributes/Data/AttributesInfo.h"
#include "GAS/Attributes/Data/AuraAttributeInfo.h"

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
}

/** Broadcast attribute info */
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo AttributeInfo = AttributesInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}

#pragma endregion CORE