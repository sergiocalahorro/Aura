// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraDamageGameplayAbility.h"

#include "AuraSummonAbility.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
class UNiagaraSystem;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraSummonAbility : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UAuraSummonAbility();

#pragma endregion INITIALIZATION

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

	/** Functionality performed when a spawned minion dies */
	UFUNCTION()
	void OnMinionDead(AActor* DestroyedActor);

private:

	/** Distance used for tracing on the Z axis when calculating a spawn location */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Centimeters"))
	float SpawnCheckTraceDistance = 400.f;

	/** Offset applied to correct spawning location on the Z axis */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Centimeters"))
	float SpawnHeightOffset = 70.f;

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
