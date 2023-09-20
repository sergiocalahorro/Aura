// Copyright Sergio Calahorro

#include "Actor/AuraEffectActor.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"

// Headers - Aura
#include "GAS/Attributes/AuraAttributeSet.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void AAuraEffectActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAuraEffectActor::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AAuraEffectActor::OnSphereEndOverlap);
}

/** Called when the game starts or when spawned */
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region CORE

/** Sphere's BeginOverlap Callback */
void AAuraEffectActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ToDo: Just for testing, remove this and modify attributes with GameplayEffects
	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(MutableAuraAttributeSet->GetHealth() + 25.f);
		MutableAuraAttributeSet->SetMana(MutableAuraAttributeSet->GetMana() - 15.f);

		Destroy();
	}
}

/** Sphere's EndOverlap Callback */
void AAuraEffectActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

#pragma endregion CORE
