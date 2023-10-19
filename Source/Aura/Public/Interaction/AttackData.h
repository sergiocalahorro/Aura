// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Headers - Aura
#include "AttackType.h"

#include "AttackData.generated.h"

class AAuraProjectile;

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()
	
public:

	/** Type of attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttackType AttackType;
	
	/** Attack's Anim Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Attack's Anim Montage Tag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackMontageTag;

	/** Combat Socket Tag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CombatSocketTag;

	/** Melee Attack's Radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "AttackType == EAttackType::Melee", EditConditionHides, ClampMin = 0.f, UIMin = 0.f, Delta = 0.1f))
	float MeleeAttackRadius = 45.f;

	/** Melee Attack's impact sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "AttackType == EAttackType::Melee", EditConditionHides))
	TObjectPtr<USoundBase> MeleeAttackImpactSound;

	/** Ranged Attack's projectile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "AttackType == EAttackType::Ranged", EditConditionHides))
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
