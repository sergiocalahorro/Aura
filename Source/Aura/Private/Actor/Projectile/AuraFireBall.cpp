// Copyright Sergio Calahorro

#include "Actor/Projectile/AuraFireBall.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "GameplayCueManager.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	
	StartMovement();
}

#pragma endregion OVERRIDES

#pragma region PROJECTILE

/** BeginOverlap Callback */
void AAuraFireBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlapValid(OtherActor))
	{
		return;
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetASC = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

/** Functionality performed when projectile hits something */
void AAuraFireBall::ProjectileHit()
{
	if (IsValid(GetOwner()))
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
	
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}

#pragma endregion PROJECTILE

#pragma region EXPLOSION

/** Fire ball's explosion */
void AAuraFireBall::Explosion()
{
	ProjectileHit();

	TArray<AActor*> ExplosionTargets;
	UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(this, GetActorLocation(), ExplosionOuterRadius, { this, GetOwner() }, ExplosionTargets);
	UAuraAbilitySystemLibrary::SetRadialDamageEffectParams(ExplosionDamageEffectParams, true, GetActorLocation(), ExplosionInnerRadius, ExplosionOuterRadius);

	for (const AActor* Target : ExplosionTargets)
	{
		const FVector DirectionToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator DirectionRotator = DirectionToTarget.Rotation();
		DirectionRotator.Pitch = 45.f;
		UAuraAbilitySystemLibrary::SetKnockbackDirectionDamageEffectParams(ExplosionDamageEffectParams, DirectionRotator.Vector(), ExplosionKnockbackMagnitude);
		UAuraAbilitySystemLibrary::SetDeathImpulseDirectionDamageEffectParams(ExplosionDamageEffectParams, DirectionRotator.Vector(), ExplosionDeathImpulseMagnitude);
	}

	Destroy();
}

#pragma endregion EXPLOSION
