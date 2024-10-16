// Copyright Sergio Calahorro

#include "Actor/Spells/PointCollection.h"

// Headers - Aura
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Point0 = CreateDefaultSubobject<USceneComponent>("Point0");
	ImmutablePoints.Add(Point0);
	SetRootComponent(Point0);

	Point1 = CreateDefaultSubobject<USceneComponent>("Point1");
	ImmutablePoints.Add(Point1);
	Point1->SetupAttachment(RootComponent);

	Point2 = CreateDefaultSubobject<USceneComponent>("Point2");
	ImmutablePoints.Add(Point2);
	Point2->SetupAttachment(RootComponent);

	Point3 = CreateDefaultSubobject<USceneComponent>("Point3");
	ImmutablePoints.Add(Point3);
	Point3->SetupAttachment(RootComponent);

	Point4 = CreateDefaultSubobject<USceneComponent>("Point4");
	ImmutablePoints.Add(Point4);
	Point4->SetupAttachment(RootComponent);

	Point5 = CreateDefaultSubobject<USceneComponent>("Point5");
	ImmutablePoints.Add(Point5);
	Point5->SetupAttachment(RootComponent);

	Point6 = CreateDefaultSubobject<USceneComponent>("Point6");
	ImmutablePoints.Add(Point6);
	Point6->SetupAttachment(RootComponent);

	Point7 = CreateDefaultSubobject<USceneComponent>("Point7");
	ImmutablePoints.Add(Point7);
	Point7->SetupAttachment(RootComponent);

	Point8 = CreateDefaultSubobject<USceneComponent>("Point8");
	ImmutablePoints.Add(Point8);
	Point8->SetupAttachment(RootComponent);

	Point9 = CreateDefaultSubobject<USceneComponent>("Point9");
	ImmutablePoints.Add(Point9);
	Point9->SetupAttachment(RootComponent);

	Point10 = CreateDefaultSubobject<USceneComponent>("Point10");
	ImmutablePoints.Add(Point10);
	Point10->SetupAttachment(RootComponent);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void APointCollection::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region POINTS

/** Get a given number of ground points based on a starting location */
TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumberOfPoints, float YawOverride, float OffsetZ, float Radius) const
{
	checkf(ImmutablePoints.Num() >= NumberOfPoints, TEXT("Attempted to access ImmutablePoints out of bounds"));

	TArray<USceneComponent*> GroundPoints;

	for (TObjectPtr<USceneComponent> Point : ImmutablePoints)
	{
		if (GroundPoints.Num() >= NumberOfPoints)
		{
			return GroundPoints;
		}

		if (Point != Point0)
		{
			FVector ToPoint = Point->GetComponentLocation() - Point0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(Point0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + OffsetZ);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - OffsetZ);

		TArray<AActor*> IgnoredActors;
		UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(this, GroundLocation, Radius, TArray<AActor*>(), IgnoredActors);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoredActors);
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		GroundPoints.Add(Point);
	}

	return GroundPoints;
}

#pragma endregion POINTS
