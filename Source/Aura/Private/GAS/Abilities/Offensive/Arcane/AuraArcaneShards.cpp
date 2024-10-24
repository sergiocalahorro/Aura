// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/Arcane/AuraArcaneShards.h"

// Headers - Unreal Engine
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

// Headers - Aura
#include "EditorDirectories.h"
#include "GameplayCueFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Actor/Spells/PointCollection.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/AbilityTasks/AbilityTask_TargetDataUnderMouse.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraArcaneShards::UAuraArcaneShards()
{
	AttackType = EAttackType::Ranged;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
/** Actually activate ability, do not call this directly */
void UAuraArcaneShards::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (K2_CheckAbilityCost())
	{
		if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>() && IsLocallyControlled())
		{
			IPlayerInterface::Execute_ShowMagicCircle(GetAvatarActorFromActorInfo(), nullptr);
		}

		WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
		WaitInputPressTask->OnPress.AddUniqueDynamic(this, &ThisClass::InputPressed);
		WaitInputPressTask->ReadyForActivation();
	}
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraArcaneShards::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WaitInputPressTask))
	{
		WaitInputPressTask->EndTask();
	}

	PointCollection->Destroy();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES

#pragma region ARCANE_SHARDS

/** Functionality performed once the input is pressed */
void UAuraArcaneShards::InputPressed(float TimeWaited)
{
	UAbilityTask_TargetDataUnderMouse* TargetDataUnderMouseTask = UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(this);
	TargetDataUnderMouseTask->ReceivedTargetData.AddUniqueDynamic(this, &UAuraArcaneShards::TargetDataReceived);
	TargetDataUnderMouseTask->ReadyForActivation();

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentAttackData.AttackMontage);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CurrentAttackData.AttackMontageTag, nullptr, true, true);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::EventReceived);
	WaitEventTask->ReadyForActivation();
}

/** Functionality performed once target data under mouse is received */
void UAuraArcaneShards::TargetDataReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	const FVector SpawnLocation = TargetDataHandle.Data.GetData()->Get()->GetHitResult()->ImpactPoint;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	NumberOfShardsCount = 0;
	NumberOfShards = FMath::Min(GetAbilityLevel(), MaxNumberOfShards);
	PointCollection = GetWorld()->SpawnActor<APointCollection>(PointCollectionClass, SpawnTransform);
	GroundPoints = PointCollection->GetGroundPoints(SpawnLocation, NumberOfShards, FMath::RandRange(0.f, 360.f));

	if (ICombatInterface* AttackingActor = CastChecked<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		AttackingActor->SetFacingTarget(SpawnLocation);
	}
}

/** Functionality performed when the ability's montage event is received */
void UAuraArcaneShards::EventReceived(FGameplayEventData Payload)
{
	GetWorld()->GetTimerManager().SetTimer(ShardSpawnTimerHandle, this, &ThisClass::SpawnShard, ShardSpawnRate, true, 0.f);

	if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_HideMagicCircle(GetAvatarActorFromActorInfo());
	}
}

/** Spawn shard */
void UAuraArcaneShards::SpawnShard()
{
	const FVector ShardSpawnLocation = GroundPoints[NumberOfShardsCount]->GetComponentLocation();

	FGameplayCueParameters CueParams;
	CueParams.Location = ShardSpawnLocation;
	UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(GetOwningActorFromActorInfo(), FAuraGameplayTags::Get().GameplayCue_ArcaneShards, CueParams);

	NumberOfShardsCount++;
	if (NumberOfShardsCount >= NumberOfShards)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShardSpawnTimerHandle);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	TArray<AActor*> TargetActors;
	UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(GetAvatarActorFromActorInfo(), ShardSpawnLocation, RadialDamageOuterRadius, { GetAvatarActorFromActorInfo() }, TargetActors);

	for (AActor* TargetActor : TargetActors)
	{
		const FVector DirectionToTarget = (ShardSpawnLocation - TargetActor->GetActorLocation()).GetSafeNormal();
		ApplyDamage(TargetActor, false, ShardSpawnLocation, true, DirectionToTarget);
	}
}

#pragma endregion ARCANE_SHARDS

#pragma region DESCRIPTION
	
/** Get ability's description */
FString UAuraArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </>"
			"<Level>%d</>\n"

			// Damage
			"<Small>Damage: </>"
			"<Damage>%d</>\n"

			// Mana Cost
			"<Small>Mana Cost: </>"
			"<ManaCost>%.1f</>\n"

			// Cooldown
			"<Small>Cooldown: </>"
			"<Cooldown>%.1f s</>\n\n"

			// Description
			"<Default>Summon a shard of arcane energy, causing radial arcane damage at the shard's location.</>"),

			// Values
			Level,
			ScaledDamage,
			ManaCost,
			Cooldown);
	}

	return FString::Printf(TEXT(
		// Title
		"<Title>ARCANE SHARDS</>\n\n"

		// Level
		"<Small>Level: </>"
		"<Level>%d</>\n"

		// Damage
		"<Small>Damage: </>"
		"<Damage>%d</>\n"

		// Mana Cost
		"<Small>Mana Cost: </>"
		"<ManaCost>%.1f</>\n"

		// Cooldown
		"<Small>Cooldown: </>"
		"<Cooldown>%.1f s</>\n\n"

		// Description
		"<Default>Summon %d shards of arcane energy, causing radial arcane damage at the shards' locations.</>"),

		// Values
		Level,
		ScaledDamage,
		ManaCost,
		Cooldown,
		FMath::Min(MaxNumberOfShards, Level));
}

/** Get ability's description for next level */
FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>ARCANE SHARDS</>\n\n"

		// Level
		"<Small>Level: </>"
		"<Level>%d</>\n"

		// Damage
		"<Small>Damage: </>"
		"<Damage>%d</>\n"

		// Mana Cost
		"<Small>Mana Cost: </>"
		"<ManaCost>%.1f</>\n"

		// Cooldown
		"<Small>Cooldown: </>"
		"<Cooldown>%.1f s</>\n\n"

		// Description
		"<Default>Summon %d shards of arcane energy, causing radial arcane damage at the shards' locations.</>"),

		// Values
		Level,
		ScaledDamage,
		ManaCost,
		Cooldown,
		FMath::Min(MaxNumberOfShards, Level));
}

#pragma endregion DESCRIPTION
