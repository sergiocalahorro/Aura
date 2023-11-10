// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Headers - Aura
#include "AttackData.h"
#include "Character/Data/CharacterClass.h"

#include "CombatInterface.generated.h"

// Forward declarations - Unreal Engine
class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FASCRegisteredSignature, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathSignature, AActor*, DeadActor);

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
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCurrentLevel();

	/** Get character's class */
	UFUNCTION(BlueprintNativeEvent)
	ECharacterClass GetCharacterClass();
	
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

	/** Get attacks of given type */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FAttackData> GetAttacksOfType(EAttackType AttackType) const;

	/** Get attack with given tag */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FAttackData GetAttackWithTag(const FGameplayTag& MontageTag) const;

	/** Get HitReact's montage */
	virtual UAnimMontage* GetHitReactMontage() const;

	/** Apply knockback */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Knockback(const FVector& KnockbackForce);

	/** Functionality performed on death */
	virtual void Death(const FVector& DeathImpulse) = 0;

	/** Whether is dead */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	/** Get blood particles' effect */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetBloodEffect() const;

	/** Get minion count */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMinionCount() const;

	/** Modify minion count by amount */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModifyMinionCount(int32 Amount);

	/** Tweening effect */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Tweening();

	/** Get delegate that is broadcasted once the ASC has been registered */
	virtual FASCRegisteredSignature GetASCRegisteredDelegate() = 0;

	/** Get delegate that is broadcasted once the actor has died */
	virtual FDeathSignature GetDeathDelegate() = 0;
};
