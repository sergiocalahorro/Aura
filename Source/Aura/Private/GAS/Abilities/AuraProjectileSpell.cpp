// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraProjectileSpell.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// Headers - Aura
#include "Actor/Projectile/AuraProjectile.h"
#include "GAS/AbilityTasks/AbilityTask_TargetDataUnderMouse.h"
#include "Interaction/CombatInterface.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CastProjectileMontage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraProjectileSpell::K2_EndAbility);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &UAuraProjectileSpell::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraProjectileSpell::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraProjectileSpell::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CastProjectileEventTag);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UAuraProjectileSpell::SpawnProjectile);
	WaitEventTask->ReadyForActivation();

	TargetDataUnderMouseTask = UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(this);
	TargetDataUnderMouseTask->ReceivedTargetData.AddUniqueDynamic(this, &UAuraProjectileSpell::TargetDataReceived);
	TargetDataUnderMouseTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraProjectileSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (IsValid(PlayMontageTask))
	{
		PlayMontageTask->EndTask();
	}

	if (IsValid(TargetDataUnderMouseTask))
	{
		TargetDataUnderMouseTask->EndTask();
	}
}

#pragma endregion OVERRIDES

#pragma region PROJECTILE

/** Cast projectile and launch it */
void UAuraProjectileSpell::SpawnProjectile(FGameplayEventData Payload)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	check(ProjectileClass);
	
	const ICombatInterface* ProjectileCaster = CastChecked<ICombatInterface>(GetAvatarActorFromActorInfo());
	
	// ToDo: Set projectile's rotation and give it a GameplayEffectSpec for causing damage
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ProjectileCaster->GetCombatSocketLocation());
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,  SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->FinishSpawning(SpawnTransform);

	if (IsValid(WaitEventTask))
	{
		WaitEventTask->EndTask();
	}
}

/** Functionality performed once target data under mouse is received */
void UAuraProjectileSpell::TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DrawDebugSphere(GetWorld(), TargetDataHandle.Get(0)->GetHitResult()->Location, 16.f, 16, FColor::Blue, false, 3.f);
}

#pragma endregion PROJECTILE