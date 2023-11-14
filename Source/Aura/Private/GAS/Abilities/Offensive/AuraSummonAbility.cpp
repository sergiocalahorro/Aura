// Copyright Sergio Calahorro

#include "GAS/Abilities/Offensive/AuraSummonAbility.h"

// Headers - Unreal Engine
#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetMathLibrary.h"

// Headers - Aura
#include "Interaction/CombatInterface.h"
#include "Utils/FunctionLibraries/HelperFunctionLibrary.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraSummonAbility::UAuraSummonAbility()
{
	AttackType = EAttackType::Summon;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraSummonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SpawnLocationIndex = 0;
	SpawnLocations = GetSpawnLocations();
	UHelperFunctionLibrary::ShuffleArray(SpawnLocations);

	for (const FVector& SpawnLocation : SpawnLocations)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CurrentAttackData.SummonEffect, SpawnLocation);
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CurrentAttackData.AttackMontage);
	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAuraSummonAbility::K2_EndAbility);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAuraSummonAbility::K2_EndAbility);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAuraSummonAbility::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CurrentAttackData.AttackMontageTag, nullptr, true, true);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UAuraSummonAbility::StartSpawning);
	WaitEventTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraSummonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

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

#pragma region SUMMON

/** Get spawn locations for summoning minions */
TArray<FVector> UAuraSummonAbility::GetSpawnLocations() const
{
	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector AvatarForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector AvatarUp = GetAvatarActorFromActorInfo()->GetActorUpVector();
	const FVector LeftOfSpread = AvatarForward.RotateAngleAxis(-CurrentAttackData.SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> CalculatedSpawnLocations;
	const int32 NumberOfMinions = FMath::RandRange(CurrentAttackData.MinNumberOfMinions, CurrentAttackData.MaxNumberOfMinions);
	const float DeltaSpread = CurrentAttackData.SpawnSpread / NumberOfMinions;
	for (int32 i = 0; i < NumberOfMinions; i++)
	{
		const FVector CurrentSpread = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector SpawnLocation = AvatarLocation + CurrentSpread * FMath::RandRange(CurrentAttackData.MinSpawnDistance, CurrentAttackData.MaxSpawnDistance);

		FHitResult Hit;
		const FVector TraceStart = SpawnLocation + AvatarUp * SpawnCheckTraceDistance;
		const FVector TraceEnd = SpawnLocation - AvatarUp * SpawnCheckTraceDistance;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			SpawnLocation = Hit.ImpactPoint;
		}
		
		CalculatedSpawnLocations.Add(SpawnLocation);
	}
	
	return CalculatedSpawnLocations;
}

/** Start spawning minions */
void UAuraSummonAbility::StartSpawning(FGameplayEventData Payload)
{
	FTimerHandle SpawnTimerHandle;
	FTimerDelegate SpawnTimerDelegate;
	SpawnTimerDelegate.BindUObject(this, &UAuraSummonAbility::SpawnMinion);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnTimerDelegate, CurrentAttackData.TimeBetweenSpawns, true);
}

/** Spawn minion */
void UAuraSummonAbility::SpawnMinion()
{
	if (!SpawnLocations.IsValidIndex(SpawnLocationIndex))
	{
		K2_EndAbility();
		return;
	}

	const TSubclassOf<APawn> SpawnedMinionClass = CurrentAttackData.MinionClasses[FMath::RandRange(0, CurrentAttackData.MinionClasses.Num() - 1)];

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector SpawnLocation = SpawnLocations[SpawnLocationIndex] + FVector(0.f, 0.f, SpawnHeightOffset);
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), SpawnLocation);
	if (APawn* SpawnedMinion = GetWorld()->SpawnActor<APawn>(SpawnedMinionClass, SpawnLocation, SpawnRotation, SpawnParameters))
	{
		SpawnedMinion->OnDestroyed.AddDynamic(this, &UAuraSummonAbility::OnMinionDead);
		SpawnedMinion->SpawnDefaultController();
		ICombatInterface::Execute_ModifyMinionCount(GetAvatarActorFromActorInfo(), 1);
		ICombatInterface::Execute_Tweening(SpawnedMinion);
		SpawnLocationIndex++;
	}
}

/** Functionality performed when a spawned minion dies */
void UAuraSummonAbility::OnMinionDead(AActor* DestroyedActor)
{
	ICombatInterface::Execute_ModifyMinionCount(GetAvatarActorFromActorInfo(), -1);
}

#pragma endregion SUMMON
