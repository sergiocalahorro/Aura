// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemLibrary.generated.h"

class AAuraHUD;
// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
struct FGameplayEffectContextHandle;

// Forward declarations - Aura
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UCharacterClassInfo;
struct FWidgetControllerParams;

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
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	/** Get Attribute Menu Widget Controller */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	/** Get Spell Menu Widget Controller */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

private:

	/** Create widget controller params */
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AAuraHUD*& OutAuraHUD);

#pragma endregion WIDGET

#pragma region CHARACTER

public:

	/** Initialize character with its default attributes' values per level */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults", meta = (DefaultToSelf = "WorldContextObject")) 
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	/** Initialize character with its default abilities */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults", meta = (DefaultToSelf = "WorldContextObject")) 
	static void GiveDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	/** Get DataAsset containing information for character class */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults", meta = (DefaultToSelf = "WorldContextObject")) 
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	/** Get XP reward for given class and level */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|CharacterDefaults", meta = (DefaultToSelf = "WorldContextObject")) 
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
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

#pragma region UTILS

	/** Get alive characters inside a sphere */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|Utils") 
	static void GetAliveCharactersWithinRadius(const UObject* WorldContextObject, const FVector& Origin, float Radius, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutAliveCharacters);

	/** Check whether an Actor is friend of another Actor (share a tag) */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|Utils")
	static bool AreActorsFriends(const AActor* FirstActor, const AActor* SecondActor);
	
#pragma endregion UTILS

};
