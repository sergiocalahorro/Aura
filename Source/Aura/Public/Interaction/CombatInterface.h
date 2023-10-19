// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Headers - Aura
#include "AttackData.h"

#include "CombatInterface.generated.h"

// Forward declarations - Unreal Engine
class UNiagaraSystem;

// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	
	/** Get Avatar that is the owner of the interface */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAvatar();

	/** Get level */
	virtual int32 GetCurrentLevel() const;
	
	/** Get socket's location that will be used in combat */
	virtual FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;

	/** Set target location to face */
	virtual void SetFacingTarget(const FVector& FacingTargetLocation);

	/** Set new combat target */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	/** Get current combat target */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	/** Get all attacks */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FAttackData> GetAllAttacks() const;

	/** Get attack with given tag */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FAttackData GetAttackWithTag(const FGameplayTag& MontageTag) const;

	/** Get HitReact's montage */
	virtual UAnimMontage* GetHitReactMontage() const;

	/** Functionality performed on death */
	virtual void Death() = 0;

	/** Whether is dead */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	/** Get blood particles' effect */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetBloodEffect() const;

	/** Get minion count */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMinionCount() const;
};
