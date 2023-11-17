// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "GAS/Abilities/AuraDamageGameplayAbility.h"

#include "AuraBeamSpell.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitInputRelease;
class UCharacterMovementComponent;

// Forward declarations - Aura
class UAbilityTask_TargetDataUnderMouse;

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UAuraBeamSpell();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion OVERRIDES

#pragma region BEAM

protected:

	/** Functionality performed once target data under mouse is received */
	UFUNCTION()
	void TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	/** Event received for spawning beam */
	UFUNCTION()
	virtual void EventReceivedSpawnBeam(FGameplayEventData Payload);

	/** Spawn beam */
	virtual void SpawnBeam();
	
	/** Spawn beams and propagate them to additional targets */
	virtual void PropagateBeamsToAdditionalTargets(AActor* InitialTarget, int32 NumAdditionalTargets);

	/** Functionality performed once the input is released */
	UFUNCTION()
	void InputReleased(float TimeHeld);

private:
	
	/** Perform trace to first target */
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	/** Get additional targets inside propagation radius */
	void GetTargetsInPropagationRadius(int32 NumAdditionalTargets, TArray<AActor*>& OutAdditionalTargets);

	/** Functionality performed when first traced target is destroyed */
	UFUNCTION()
	void OnFirstTargetDestroyed(AActor* DestroyedActor);

	/** Functionality performed when an additional target is destroyed */
	UFUNCTION()
	void OnAdditionalTargetDestroyed(AActor* DestroyedActor);

	/** Destroy beams */
	void DestroyBeams();

protected:

	/** Socket's name used for spawning the beam */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam")
	FName BeamStartSocket;

	/** GameplayCue's tag added when the beam starts */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam")
	FGameplayTag CueBeamStart;

	/** GameplayCue's tag added when the beam loops */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam")
	FGameplayTag CueBeamLoop;

	/** Radius used for first beam's trace to target */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam", meta = (ClampMin = 0.f, UIMin = 0.f, Delta = 1.f, Units = "Centimeters"))
	float BeamTraceRadius = 10.f;

	/** Maximum number of targets that can be affected by beam's propagation */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam", meta = (ClampMin = 1, UIMin = 1, ClampMax = 5, UIMax = 5))
	int32 MaxPropagationTargets = 5;

	/** Radius used for beam's propagation from first target */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam", meta = (ClampMin = 0.f, UIMin = 0.f, Delta = 1.f, Units = "Centimeters"))
	float BeamPropagationRadius = 300.f;

	/** Minimum time that needs to be elapsed in order to end ability when releasing input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Beam", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "Seconds"))
	float MinSpellTime = 0.5f;
	
	/** Location hit by mouse trace */
	UPROPERTY()
	FVector MouseHitLocation;

	/** Actor hit by mouse trace */
	UPROPERTY()
	TObjectPtr<AActor> MouseHitActor;

	/** Owner's player controller */
	UPROPERTY()
	TObjectPtr<APlayerController> OwnerPlayerController;

	/** Owner's character movement component */
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

private:

	/** Target for the first beam cue */
	UPROPERTY()
	AActor* FirstTarget;

	/** First target's cue parameters */
	UPROPERTY()
	FGameplayCueParameters FirstTargetCueParams;

	/** Cue parameters mapped to additional targets */
	UPROPERTY()
	TMap<AActor*, FGameplayCueParameters> AdditionalTargetsToCueParams;

#pragma endregion BEAM

#pragma region DAMAGE

private:
	
	/** Apply damage and cost */
	void ApplyDamageAndCost();

protected:

	/** Time rate for applying damage and committing cost */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Damage", meta = (ClampMin = 0.f, UIMin = 0.f))
	float DamageAndCostRate = 0.1f;

private:

	/** Timer used to apply cost and damage */
	UPROPERTY()
	FTimerHandle DamageCostTimerHandle;

#pragma endregion DAMAGE

#pragma region ABILITY

private:

	/** Ability task used for playing the Montage for casting the beam spell */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	/** Ability task used for waiting for a GameplayTag event */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;

	/** Ability task used for retrieving target data under mouse */
	UPROPERTY()
	TObjectPtr<UAbilityTask_TargetDataUnderMouse> TargetDataUnderMouseTask;

	/** Ability task used for waiting the input to be released */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputRelease> WaitInputReleaseTask;

#pragma endregion ABILITY
	
};
