// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AuraArcaneShards.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_WaitInputPress;

// Forward declarations - Aura
class APointCollection;

/**
 * 
 */
UCLASS()
class AURA_API UAuraArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

	/** Sets default values for this object's properties */
	UAuraArcaneShards();

#pragma endregion INITIALIZATION
	
#pragma region OVERRIDES
	
protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region ARCANE_SHARDS

private:

	/** Functionality performed once the input is pressed */
	UFUNCTION()
	void InputPressed(float TimeWaited);

	/** Functionality performed once target data under mouse is received */
	UFUNCTION()
	void TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	/** Functionality performed when the ability's montage event is received */
	UFUNCTION()
	void EventReceived(FGameplayEventData Payload);

	/** Spawn shard */
	void SpawnShard();

protected:

	/** Class of point collection to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "AA|ArcaneShards")
	TSubclassOf<APointCollection> PointCollectionClass;
	
	/** Maximum number of shards */
	UPROPERTY(EditDefaultsOnly, Category = "AA|ArcaneShards", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxNumberOfShards = 10;

	/** Time rate at which shards are spawned */
	UPROPERTY(EditDefaultsOnly,  Category = "AA|ArcaneShards", meta = (ClampMin = 0.f, UIMin = 0.f))
	float ShardSpawnRate = 0.1f;

private:

	/** Spawned point collection actor */
	UPROPERTY()
	TObjectPtr<APointCollection> PointCollection;

	/** Ground points at which shards will be spawned */
	UPROPERTY()
	TArray<USceneComponent*> GroundPoints;

	/** Number of shards to be spawned */
	int32 NumberOfShards = 0;

	/** Current spawned number of shards */
	int32 NumberOfShardsCount = 0;

	/** Shards' spawning timer */
	FTimerHandle ShardSpawnTimerHandle;

#pragma endregion ARCANE_SHARDS

#pragma region ABILITY

private:

	/** Ability task used for waiting the input to be pressed */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputPress> WaitInputPressTask;

#pragma endregion ABILITY

#pragma region DESCRIPTION

public:
	
	/** Get ability's description */
	virtual FString GetDescription(int32 Level) override;

	/** Get ability's description for next level */
	virtual FString GetNextLevelDescription(int32 Level) override;

#pragma endregion DESCRIPTION
	
};
