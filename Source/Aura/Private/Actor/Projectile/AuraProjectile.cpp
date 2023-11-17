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
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);

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
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	
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
	if (!DamageEffectParams.SourceASC)
	{
		return;
	}
	
	const AActor* SourceAvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceAvatarActor == OtherActor || UAuraAbilitySystemLibrary::AreActorsFriends(SourceAvatarActor, OtherActor))
	{
		return;
	}

	ProjectileHit();
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			const bool bKnockBack = FMath::RandRange(1.f, 100.f) < DamageEffectParams.KnockbackChance;
			const FVector KnockbackDirection = FRotator(45.f, GetActorRotation().Yaw, GetActorRotation().Roll).Vector();
			const FVector KnockbackForce = bKnockBack ? KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude : FVector::ZeroVector;
			DamageEffectParams.KnockbackForce = KnockbackForce;

			DamageEffectParams.TargetASC = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
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
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}

#pragma endregion PROJECTILE

#pragma region HOMING

/** Set projectile's homing target */
void AAuraProjectile::SetHomingTarget(AActor* HomingTarget)
{
	if (IsValid(DamageEffectParams.SourceASC))
	{
		if (HomingTarget == DamageEffectParams.SourceASC->GetAvatarActor())
		{
			return;
		}
	}

	if (IsValid(HomingTarget))
	{
		HomingTargetComponent = HomingTarget->GetRootComponent();
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HomingTarget))
		{
			CombatInterface->GetDeathDelegate().AddUniqueDynamic(this, &AAuraProjectile::OnHomingTargetDestroyed);
		}
	}

	SetHomingBehaviour();
}

/** Set projectile's homing target location */
void AAuraProjectile::SetHomingTargetLocation(const FVector& TargetLocation)
{
	HomingTargetComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
	HomingTargetComponent->SetWorldLocation(TargetLocation);
	SetHomingBehaviour();
}

/** Set projectile's homing behaviour */
void AAuraProjectile::SetHomingBehaviour() const
{
	ProjectileMovementComponent->HomingTargetComponent = HomingTargetComponent;
	ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(ProjectileData->MinHomingAcceleration, ProjectileData->MaxHomingAcceleration);
	ProjectileMovementComponent->bIsHomingProjectile = true;
}

/** Functionality performed once the homing target is destroyed */
void AAuraProjectile::OnHomingTargetDestroyed(AActor* DeadActor)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(DeadActor))
	{
		CombatInterface->GetDeathDelegate().RemoveDynamic(this, &AAuraProjectile::OnHomingTargetDestroyed);
	}

	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	ProjectileMovementComponent->bIsHomingProjectile = false;
}

#pragma endregion HOMING