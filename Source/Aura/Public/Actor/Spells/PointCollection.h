// Copyright Sergio Calahorro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values for this actor's properties */
	APointCollection();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region POINTS

public:

	/** Get a given number of ground points based on a starting location */
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumberOfPoints, float YawOverride = 0.f, float OffsetZ = 500.f, float Radius = 1500.f) const;

protected:

	/** Point 0 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point0;

	/** Point 1 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point1;

	/** Point 2 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point2;

	/** Point 3 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point3;

	/** Point 4 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point4;

	/** Point 5 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point5;

	/** Point 6 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point6;

	/** Point 7 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point7;

	/** Point 8 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point8;

	/** Point 9 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point9;

	/** Point 10 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Point10;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> ImmutablePoints; 

#pragma endregion POINTS

};
