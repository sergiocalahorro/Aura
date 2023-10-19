// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAuraSummonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetSpawnLocations();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAuraSummonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
	
#pragma endregion OVERRIDES

#pragma region SUMMON

/** Get spawn locations for summoning minions */
TArray<FVector> UAuraSummonAbility::GetSpawnLocations() const
{
	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector AvatarForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector LeftOfSpread = AvatarForward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	const int32 NumberOfMinions = FMath::RandRange(MinNumberOfMinions, MaxNumberOfMinions);
	const float DeltaSpread = SpawnSpread / NumberOfMinions;
	for (int32 i = 0; i < NumberOfMinions; i++)
	{
		const FVector CurrentSpread = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);

		UKismetSystemLibrary::DrawDebugArrow(this, AvatarLocation, AvatarLocation + CurrentSpread * MaxSpawnDistance, 4.f, FLinearColor::Blue, 3.f);
		DrawDebugSphere(GetWorld(), AvatarLocation + CurrentSpread * MinSpawnDistance, 10.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), AvatarLocation + CurrentSpread * MaxSpawnDistance, 10.f, 12, FColor::Red, false, 3.f);

		const FVector SpawnLocation = AvatarLocation + CurrentSpread * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		SpawnLocations.Add(SpawnLocation);

		DrawDebugSphere(GetWorld(), SpawnLocation, 20.f, 12, FColor::White, false, 3.f);
	}
	
	return SpawnLocations;
}

#pragma endregion SUMMON
