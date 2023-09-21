// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "GAS/Effects/EffectProperties.h"

#include "AuraAttributeSet.generated.h"

// Forward declarations - Unreal Engine
struct FGameplayEffectModCallbackData;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values for this object's properties */
	UAuraAttributeSet();
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called just before any modification happens to an attribute */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;

#pragma endregion OVERRIDES

#pragma region ATTRIBUTES

public:

	/** Health's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	/** MaxHealth's Replication Notify Callback */
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	/** Mana's Replication Notify Callback */
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	/** MaxMana's Replication Notify Callback */
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

public:

	/** Health Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "AA|Vital|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)

	/** MaxHealth Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "AA|Vital|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	/** Mana Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "AA|Vital|Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	/** MaxMana Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "AA|Vital|Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

#pragma endregion ATTRIBUTES

#pragma region EFFECTS

private:

	/** Populate structure containing the effect's properties */
	void SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Properties) const;

#pragma endregion EFFECTS
	
};
