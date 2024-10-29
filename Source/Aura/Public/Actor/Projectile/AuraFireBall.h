// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraProjectile.h"

#include "AuraFireBall.generated.h"

UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region PROJECTILE

protected:
	
	/** BeginOverlap Callback */
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
#pragma endregion PROJECTILE

#pragma region FIREBALL

public:

	/** Start fire ball's movement */
	UFUNCTION(BlueprintImplementableEvent)
	void StartMovement();

private:

	/** Distance the fire ball will travel to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|FireBall", meta = (AllowPrivateAccess = "true", Units = "Centimeters"))
	float TravelDistance = 800.f;

	/** Distance from the instigator at which the fire ball will explode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|FireBall", meta = (AllowPrivateAccess = "true", Units = "Centimeters"))
	float ExplodeDistance = 150.f;

#pragma endregion FIREBALL
};
