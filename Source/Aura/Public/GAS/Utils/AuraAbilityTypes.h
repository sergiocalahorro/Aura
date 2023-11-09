// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "AuraAbilityTypes.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

public:
	
	/** Default constructor */
	FDamageEffectParams()
	{
		
	}

public:

	/** World context object */
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;

	/** Damage effect's class */
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Source's Ability System Component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;
	
	/** Target's Ability System Component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	/** Base damage value */
	UPROPERTY()
	float BaseDamage = 0.f;
	
	/** Ability's level */
	UPROPERTY()
	float AbilityLevel = 1.f;

	/** Damage type tag */
	UPROPERTY()
	FGameplayTag DamageType;
	
	/** Chance to apply debuff */
	UPROPERTY()
	float DebuffChance = 0.f;

	/** Damage caused when debuff is applied */
	UPROPERTY()
	float DebuffDamage = 0.f;

	/** Duration for the applied debuff */
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	/** Frequency for the applied debuff */
	UPROPERTY()
	float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
public:

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

#pragma endregion OVERRIDES

#pragma region EFFECT

public:

	/** Return whether the damage effect associated is blocked */
	bool IsBlockedHit() const { return bIsBlockedHit; }

	/** Return whether the damage effect associated is critical */
	bool IsCriticalHit() const { return bIsCriticalHit; }

	/** Return whether the damage effect associated can apply a debuff successfully */
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }

	/** Return debuff's damage */
	float GetDebuffDamage() const { return DebuffDamage; }

	/** Return debuff's duration */
	float GetDebuffDuration() const { return DebuffDuration; }

	/** Return debuff's frequency */
	float GetDebuffFrequency() const { return DebuffFrequency; }
	
	/** Return the damage type associated to this effect */
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }

	/** Set whether the damage effect associated is blocked */
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	/** Set whether the damage effect associated is critical */
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	/** Set whether the damage effect associated can apply a debuff successfully */
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }

	/** Set debuff's damage */
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }

	/** Set debuff's duration */
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }

	/** Set debuff's frequency */
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }

	/** Set damage type */
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }

protected:

	/** Whether the damage effect associated is blocked */
	UPROPERTY()
	bool bIsBlockedHit = false;

	/** Whether the damage effect associated is critical */
	UPROPERTY()
	bool bIsCriticalHit = false;

	/** Whether the damage effect associated can apply a debuff successfully */
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	/** Debuff's damage */
	UPROPERTY()
	float DebuffDamage = 0.f;

	/** Debuff's duration */
	UPROPERTY()
	float DebuffDuration = 0.f;

	/** Debuff's frequency */
	UPROPERTY()
	float DebuffFrequency = 0.f;

	/** Damage type tag */
	TSharedPtr<FGameplayTag> DamageType;

#pragma endregion EFFECT
	
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};