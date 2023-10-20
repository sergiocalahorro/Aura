// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Headers - Aura
#include "AttackType.h"

#include "AttackData.generated.h"

// Forward declarations - Unreal Engine
class UNiagaraSystem;

// Forward declarations - Aura
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

	/** Minion classes to summon */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides))
	TArray<TSubclassOf<APawn>> MinionClasses;

	/** Minimum number of minions to summon */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides, ClampMin = 2, UIMin = 2, ClampMax = 10, UIMax = 10))
	int32 MinNumberOfMinions = 3;

	/** Maximum number of minions to summon */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides, ClampMin = 2, UIMin = 2, ClampMax = 10, UIMax = 10))
	int32 MaxNumberOfMinions = 5;
	
	/** Minimum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides, ClampMin = 0.f, UIMin = 0.f, ClampMax = 1000.f, UIMax = 1000.f, Delta = 1.f, Units = "Centimeters"))
	float MinSpawnDistance = 150.f;

	/** Maximum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides, ClampMin = 0.f, UIMin = 0.f, ClampMax = 1000.f, UIMax = 1000.f, Delta = 1.f, Units = "Centimeters"))
	float MaxSpawnDistance = 500.f;

	/** Maximum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides, ClampMin = 0.f, UIMin = 0.f, ClampMax = 360.f, UIMax = 360.f, Delta = 1.f, Units = "Degrees"))
	float SpawnSpread = 90.f;
	
	/** Time between minion spawns */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides, ClampMin = 0.f, UIMin = 0.f, Units = "Seconds"))
	float TimeBetweenSpawns = 0.2f;
	
	/** Summon effect particles */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AttackType == EAttackType::Summon", EditConditionHides))
	TObjectPtr<UNiagaraSystem> SummonEffect;
};
