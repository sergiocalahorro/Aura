// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemLibrary.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
struct FGameplayEffectContextHandle;

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
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	/** Get Attribute Menu Widget Controller */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

#pragma endregion WIDGET

#pragma region CHARACTER

public:

	/** Initialize character with its default attributes' values per level */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults") 
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	/** Initialize character with its default abilities */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults") 
	static void GiveDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	/** Get DataAsset containing information for character class */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults") 
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
#pragma endregion CHARACTER

#pragma region EFFECT

public:

	/** Return whether damage effect is blocked */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Return whether damage effect is critical hit */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	/** Set whether damage effect is blocked */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	/** Set whether damage effect is critical hit */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

#pragma endregion EFFECT

};
