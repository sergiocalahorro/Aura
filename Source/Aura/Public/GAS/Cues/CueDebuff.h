// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"

#include "CueDebuff.generated.h"

// Forward declarations - Unreal Engine
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API ACueDebuff : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	ACueDebuff();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called when a GameplayCue with duration is first activated, this will only be called if the client witnessed the activation */
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	/** Called when a GameplayCue with duration is removed */
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

#pragma endregion OVERRIDES

#pragma region DEBUFF

private:

	/** Called when target is destroyed */
	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);

	/** Callback called when the debuff's particle system is finished */
	UFUNCTION()
	void OnDebuffSystemFinished(UNiagaraComponent* PSystem);

protected:

	/** Particle system */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Debuff")
	TObjectPtr<UNiagaraSystem> ParticleSystem;

private:

	/** Particle system's compoennt */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ParticleSystemComponent;

#pragma endregion DEBUFF
	
};
