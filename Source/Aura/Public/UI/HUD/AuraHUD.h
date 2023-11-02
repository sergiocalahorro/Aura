// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "AuraHUD.generated.h"

// Forward declarations - Unreal Engine
class UAttributeSet;
class UAbilitySystemComponent;

// Forward declarations - Aura
class UAuraUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

#pragma region OVERLAY

public:

	/** Initialize Overlay */
	void InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet);

	/** Get Overlay Widget Controller */
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

private:

	/** Overlay Widget */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Overlay")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	/** Overlay Widget Controller class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	/** Overlay Widget */
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	/** Overlay Widget Controller */
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

#pragma endregion OVERLAY

#pragma region ATTRIBUTE_MENU

public:
	
	/** Get Attribute Menu Widget Controller */
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

private:

	/** Attribute Menu Controller class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|AttributeMenu")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	/** Attribute Menu Widget Controller */
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

#pragma endregion ATTRIBUTE_MENU

#pragma region SPELL_MENU

public:
	
	/** Get Spell Menu Widget Controller */
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

private:

	/** Spell Menu Controller class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|SpellMenu")
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
	/** Spell Menu Widget Controller */
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

#pragma endregion SPELL_MENU
	
};
