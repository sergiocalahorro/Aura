// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/AuraRangedAttack.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "Actor/Projectile/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraRangedAttack::UAuraRangedAttack()
{
	AttackType = EAttackType::Ranged;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraRangedAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const AActor* CombatTarget = ICombatInterface::Execute_GetCombatTarget(ActorInfo->AvatarActor.Get()))
	{
		if (ICombatInterface* AttackingActor = Cast<ICombatInterface>(ActorInfo->AvatarActor.Get()))
		{
			AttackingActor->SetFacingTarget(CombatTarget->GetActorLocation());
			ProjectileTargetLocation = CombatTarget->GetActorLocation();
		}
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentAttackData.AttackMontage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraRangedAttack::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraRangedAttack::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraRangedAttack::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CurrentAttackData.AttackMontageTag, nullptr, true, true);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UAuraRangedAttack::EventReceivedSpawnProjectile);
	WaitEventTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraRangedAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(PlayMontageTask))
	{
		PlayMontageTask->EndTask();
	}

	if (IsValid(WaitEventTask))
	{
		WaitEventTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region RANGED_ATTACK

/** Event received for spawning projectile */
void UAuraRangedAttack::EventReceivedSpawnProjectile(FGameplayEventData Payload)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	const ICombatInterface* AttackingActor = CastChecked<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector SpawnLocation = AttackingActor->GetCombatSocketLocation(CurrentAttackData.CombatSocketTag);
	const FRotator SpawnRotation = (ProjectileTargetLocation - SpawnLocation).Rotation();

	SpawnProjectile(CurrentAttackData.ProjectileClass, SpawnLocation, SpawnRotation, bOverridePitch, PitchOverride);
}

/** Spawn projectile */
void UAuraRangedAttack::SpawnProjectile(TSubclassOf<AAuraProjectile> ProjectileClass, const FVector& SpawnLocation, const FRotator& SpawnRotation, bool bInOverridePitch, float InPitchOverride, AActor* HomingTarget) const
{
	check(ProjectileClass);

	FRotator FinalSpawnRotation = SpawnRotation;
	if (bInOverridePitch)
	{
		FinalSpawnRotation.Pitch = InPitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(FinalSpawnRotation.Quaternion());
	
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	const FDamageEffectParams DamageEffectParams = MakeDamageEffectParams();
	Projectile->DamageEffectParams = DamageEffectParams;

	if (IsValid(HomingTarget) && HomingTarget->Implements<UCombatInterface>())
	{
		Projectile->SetHomingTarget(HomingTarget);
	}
	else
	{
		Projectile->SetHomingTargetLocation(ProjectileTargetLocation);
	}

	Projectile->FinishSpawning(SpawnTransform);
}


#pragma endregion RANGED_ATTACK

