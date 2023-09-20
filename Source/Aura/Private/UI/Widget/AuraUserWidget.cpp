// Copyright Sergio Calahorro

#include "UI/Widget/AuraUserWidget.h"

#pragma region CORE

/** Get WidgetController */
UObject* UAuraUserWidget::GetWidgetController() const
{
	return WidgetController;
}

/** Set WidgetController */
void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

/** Functionality performed once WidgetController is set */
void UAuraUserWidget::OnWidgetControllerSet_Implementation()
{
	
}

#pragma endregion CORE