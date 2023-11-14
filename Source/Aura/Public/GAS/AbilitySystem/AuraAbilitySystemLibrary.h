// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UGameplayEffect;
// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
struct FGameplayEffectContextHandle;

// Forward declarations - Aura
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UCharacterClassInfo;
class UAbilitiesInfo;
class AAuraHUD;
struct FWidgetControllerParams;
struct FDamageEffectParams;


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

#pragma region ABILITIES

public:
	
	/** Get DataAsset containing information for abilities */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|Abilities", meta = (DefaultToSelf = "WorldContextObject")) 
	static UAbilitiesInfo* GetAbilitiesInfo(const UObject* WorldContextObject);

#pragma endregion ABILITIES

#pragma region EFFECTS

public:

	/** Apply effect to self */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|Effect")
	static FGameplayEffectContextHandle ApplyEffectToSelf(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel = 1.f);

	/** Apply damage effect to target */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
	
	/** Return whether damage effect is blocked */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Return whether damage effect is critical hit */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Return whether damage effect can apply debuff successfully */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Return debuff's damage */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Return debuff's duration */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Return debuff's frequency */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	
	/** Get the effect context's damage type */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Get the effect context's death impulse */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	/** Get the effect context's knockback force */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	/** Set whether damage effect is blocked */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	/** Set whether damage effect is critical hit */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	/** Set whether damage effect can apply debuff successfully */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	/** Set debuff's damage */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	/** Set debuff's duration */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	/** Set debuff's frequency */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	/** Set damage type */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	/** Set death impulse */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);

	/** Set knockback force */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|EffectContext")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);
	
#pragma endregion EFFECTS

#pragma region UTILS

	/** Get alive characters inside a sphere */
	UFUNCTION(BlueprintCallable, Category = "AA|AuraAbilitySystemLibrary|Utils") 
	static void GetAliveCharactersWithinRadius(const UObject* WorldContextObject, const FVector& Origin, float Radius, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutAliveCharacters);

	/** Check whether an Actor is friend of another Actor (share a tag) */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|Utils")
	static bool AreActorsFriends(const AActor* FirstActor, const AActor* SecondActor);

	/** Get rotations rotated from a starting forward vector, rotated around the given axis with a given spread */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|Utils")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& ForwardVector, const FVector& AxisToRotateAround, float Spread, int32 NumberOfRotators);

	/** Get vectors rotated from a starting forward vector, rotated around the given axis with a given spread */
	UFUNCTION(BlueprintPure, Category = "AA|AuraAbilitySystemLibrary|Utils")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& ForwardVector, const FVector& AxisToRotateAround, float Spread, int32 NumberOfVectors);
	
#pragma endregion UTILS

};
