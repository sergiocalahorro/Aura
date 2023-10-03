// Copyright Sergio Calahorro

#include "UI/HUD/AuraHUD.h"

// Headers - Unreal Engine
#include "Blueprint/UserWidget.h"

// Headers - Aura
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/WidgetControllerParams.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

#pragma region OVERLAY

/** Initialize overlay */
void AAuraHUD::InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("AAuraHUD::InitOverlay - OverlayWidgetClass unitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("AAuraHUD::InitOverlay - OverlayWidgetControllerClass unitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = CastChecked<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(InPlayerController, InPlayerState, InAbilitySystemComponent, InAttributeSet);
	OverlayWidget->SetWidgetController(GetOverlayWidgetController(WidgetControllerParams));
	OverlayWidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

/** Get overlay widget controller */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDelegates();
	}

	return OverlayWidgetController;
}

#pragma endregion OVERLAY

#pragma region ATTRIBUTE_MENU

/** Get Attribute Menu Widget Controller */
UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AttributeMenuWidgetController->BindCallbacksToDelegates();
	}

	return AttributeMenuWidgetController;
}

#pragma endregion ATTRIBUTE_MENU
