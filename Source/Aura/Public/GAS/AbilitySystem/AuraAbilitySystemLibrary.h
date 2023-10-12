// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemLibrary.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;

// Forward declarations - Aura
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region WIDGET
	
public:

	/** Get Overlay Widget Controller */
	UFUNCTION(BlueprintPure, Category = "AA|Widget|Controller")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	/** Get Attribute Menu Widget Controller */
	UFUNCTION(BlueprintPure, Category = "AA|Widget|Controller")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

#pragma endregion WIDGET

#pragma region CHARACTER

	/** Initialize character with its default attributes' values per level */
	UFUNCTION(BlueprintCallable, Category = "AA|Character|Defaults") 
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	/** Initialize character with its default abilities */
	UFUNCTION(BlueprintCallable, Category = "AA|Character|Defaults") 
	static void GiveDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);

	/** Get DataAsset containing information for character class */
	UFUNCTION(BlueprintCallable, Category = "AA|Character|Defaults") 
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
#pragma endregion CHARACTER

};
