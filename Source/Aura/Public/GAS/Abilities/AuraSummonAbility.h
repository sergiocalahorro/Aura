// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraGameplayAbility.h"

#include "AuraSummonAbility.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1000.f, UIMax = 1000.f, Delta = 1.f))
	float MinSpawnDistance = 150.f;

	/** Maximum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1000.f, UIMax = 1000.f, Delta = 1.f))
	float MaxSpawnDistance = 500.f;

	/** Maximum distance from caster to summon minions */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Summon", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 360.f, UIMax = 360.f, Delta = 1.f))
	float SpawnSpread = 90.f;

#pragma endregion SUMMON
};
