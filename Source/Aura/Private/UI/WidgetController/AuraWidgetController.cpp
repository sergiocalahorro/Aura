// Copyright Sergio Calahorro

#include "UI/WidgetController/AuraWidgetController.h"

// Headers - Aura
#include "UI/WidgetController/WidgetControllerParams.h"

#pragma region CORE

/** Set widget controller's params */
void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

#pragma endregion CORE