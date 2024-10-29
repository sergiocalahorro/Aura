// Copyright Sergio Calahorro

#include "Actor/Projectile/AuraFireBall.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"

// Headers - Aura
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

#pragma endregion PROJECTILE
