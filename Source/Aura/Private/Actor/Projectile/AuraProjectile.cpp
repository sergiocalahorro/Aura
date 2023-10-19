// Copyright Sergio Calahorro

#include "Actor/Projectile/AuraProjectile.h"

// Headers - Unreal Engine
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"

// Headers - Aura
#include "Aura.h"
#include "Actor/Projectile/ProjectileData.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	ProjectileVFX->SetupAttachment(Sphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void AAuraProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAuraProjectile::OnBeginOverlap);
}

/** Called when an instance of this class is placed (in editor) or spawned */
void AAuraProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InitializeProjectile();
}

/** Called when the game starts or when spawned */
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(ProjectileData->LifeSpan);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(ProjectileData->LoopingSound, GetRootComponent());
}

/** Called when this actor is explicitly being destroyed during gameplay or in the editor, not called during level streaming or gameplay ending */
void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		ProjectileHit();
	}

	Super::Destroyed();
}

#pragma endregion OVERRIDES

#pragma region PROJECTILE

/** BeginOverlap Callback */
void AAuraProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}

	if (UAuraAbilitySystemLibrary::AreActorsFriends(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}

	ProjectileHit();
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

/** Initialize projectile's values */
void AAuraProjectile::InitializeProjectile() const
{
	if (ProjectileData)
	{
		ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
		ProjectileMovementComponent->ProjectileGravityScale = ProjectileData->GravityScale;
		ProjectileMovementComponent->bInitialVelocityInLocalSpace = ProjectileData->bInitialVelocityInLocalSpace;
		ProjectileMovementComponent->bRotationFollowsVelocity = ProjectileData->bRotationFollowsVelocity;
		ProjectileMovementComponent->bShouldBounce = ProjectileData->bShouldBounce;
		ProjectileMovementComponent->Bounciness = ProjectileData->Bounciness;

		ProjectileVFX->SetAsset(ProjectileData->ProjectileVFX);
	}
}

/** Functionality performed when projectile hits something */
void AAuraProjectile::ProjectileHit()
{
	if (bHit)
	{
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, ProjectileData->ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileData->ImpactEffect, GetActorLocation());

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}

	bHit = true;
}

#pragma endregion PROJECTILE
