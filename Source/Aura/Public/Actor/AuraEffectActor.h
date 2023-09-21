// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AuraEffectActor.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
struct FActiveGameplayEffectHandle;

// Forward declarations - Aura
struct FEffectInfo;

UCLASS(Abstract)
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AAuraEffectActor();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region EFFECTS

protected:

	/** Apply effect to target Actor */
	FActiveGameplayEffectHandle ApplyEffectToTarget(AActor* Target, const TSubclassOf<UGameplayEffect>& EffectClass) const;

	/** Remove active effect from target Actor */
	bool RemoveActiveEffectFromTarget(AActor* Target, const FActiveGameplayEffectHandle& ActiveEffectHandle) const;
	
	/** Box's BeginOverlap Callback */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Box's EndOverlap Callback */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:

	/** Effects to apply */
	UPROPERTY(EditAnywhere, Category = "AA|Effect")
	TArray<FEffectInfo> Effects;

	/** Handles for active applied effects */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	/** Whether Actor should be destroyed on effects' removal */
	UPROPERTY(EditAnywhere, Category = "AA|Effect")
	bool bDestroyOnEffectsRemoval = false;

#pragma endregion EFFECTS
	
};
