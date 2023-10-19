// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraGameplayAbility.h"
#include "Interaction/AttackData.h"

#include "AuraSummonAbility.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
class UNiagaraSystem;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region SUMMON
	
private:

	/** Get spawn locations for summoning minions */
	TArray<FVector> GetSpawnLocations() const;

	/** Start spawning minions */
	UFUNCTION()
	void StartSpawning(FGameplayEventData Payload);

	/** Spawn minion */
	UFUNCTION()
	void SpawnMinion();

private:
	
	/** Minion classes to summon */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	/** Minimum number of minions to summon */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 2, UIMin = 2, ClampMax = 10, UIMax = 10))
	int32 MinNumberOfMinions = 3;

	/** Maximum number of minions to summon */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 2, UIMin = 2, ClampMax = 10, UIMax = 10))
	int32 MaxNumberOfMinions = 5;
	
	/** Minimum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1000.f, UIMax = 1000.f, Delta = 1.f, Units = "Centimeters"))
	float MinSpawnDistance = 150.f;

	/** Maximum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1000.f, UIMax = 1000.f, Delta = 1.f, Units = "Centimeters"))
	float MaxSpawnDistance = 500.f;

	/** Maximum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 360.f, UIMax = 360.f, Delta = 1.f, Units = "Degrees"))
	float SpawnSpread = 90.f;
	
	/** Time between minion spawns */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Seconds"))
	float TimeBetweenSpawns = 0.2f;

	/** Distance used for tracing on the Z axis when calculating a spawn location */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Centimeters"))
	float SpawnCheckTraceDistance = 400.f;

	/** Offset applied to correct spawning location on the Z axis */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Centimeters"))
	float SpawnHeightOffset = 70.f;
	
	/** Summon effect particles */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon")
	TObjectPtr<UNiagaraSystem> SummonEffect;

	/** Calculated spawn locations */
	TArray<FVector> SpawnLocations;
	
	/** Current spawn location index */
	int32 SpawnLocationIndex;

#pragma endregion SUMMON

#pragma region ABILITY

private:

	/** Ability task used for playing the Montage for performing the summon */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	/** Ability task used for waiting for a GameplayTag event */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;

#pragma endregion ABILITY
	
};
