// Copyright Sergio Calahorro

#include "UI/WidgetController/AuraWidgetController.h"

// Headers - Aura
#include "UI/WidgetController/WidgetControllerParams.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/AbilitySystem/Data/AbilitiesInfo.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

#pragma region CORE

/** Set widget controller's params */
void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

/** Getter of AuraPlayerController */
AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
	if (!AuraPlayerController)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}

	return AuraPlayerController;
}

/** Getter of AuraPlayerState */
AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (!AuraPlayerState)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}

	return AuraPlayerState;
}

/** Getter of AuraAbilitySystemComponent */
UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}

	return AuraAbilitySystemComponent;
}

/** Getter of AuraAttributeSet */
UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (!AuraAttributeSet)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}

	return AuraAttributeSet;
}

#pragma endregion CORE

#pragma region ABILITIES

/** Broadcast ability info */
void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
	{
		return;
	}

	FBroadcastAbilitySignature BroadcastAbilityDelegate;
	BroadcastAbilityDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo AbilityInfo = AbilitiesInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			AbilityInfo.InputTag = AuraAbilitySystemComponent->GetAbilityInputTagFromSpec(AbilitySpec);
			AbilityInfo.StatusTag = AuraAbilitySystemComponent->GetAbilityStatusTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		}
	);

	AuraAbilitySystemComponent->BroadcastAbility(BroadcastAbilityDelegate);
}

#pragma endregion ABILITIES