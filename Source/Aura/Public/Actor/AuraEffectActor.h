// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"

#include "AuraEffectActor.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;

// Forward declarations - Aura
struct FEffectDefinition;

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
	void ApplyEffectToTarget(AActor* Target, const FEffectDefinition& EffectInfo);
	
	/** BeginOverlap Callback */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** EndOverlap Callback */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:

	/** Actor Level used when applying effects (this value overrides the levels set individually on the effects) */
	UPROPERTY(EditAnywhere, Category = "AA|Effect", meta = (ClampMin = 1.f, UIMin = 1.f, ClampMax = 10.f, UIMax = 10.f, Delta = 1.f))
	float ActorLevel = 1.f;

	/** Effects to apply */
	UPROPERTY(EditAnywhere, Category = "AA|Effect")
	TArray<FEffectDefinition> Effects;

	/** Whether Actor should be destroyed on effects' application */
	UPROPERTY(EditAnywhere, Category = "AA|Effect")
	bool bDestroyOnEffectsApplication = false;

	/** Whether Actor's effects should be affect enemies */
	UPROPERTY(EditAnywhere, Category = "AA|Effect")
	bool bShouldAffectEnemies = false;

	/** Active effects' handles mapped to the ASCs they are applied to */
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

#pragma endregion EFFECTS
	
};
