// Copyright Sergio Calahorro

#include "Actor/AuraEffectActor.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "GAS/Effects/EffectDefinition.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region EFFECTS

/** Apply effect to target Actor */
void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, const FEffectDefinition& EffectInfo)
{
	check(EffectInfo.EffectClass);

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectInfo.EffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// Store active effect handle if the effect is infinite and should be removed on EndOverlap
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && EffectInfo.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

/** BeginOverlap Callback */
void AAuraEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (FEffectDefinition& EffectInfo : Effects)
	{
		// Apply effect
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
		{
			ApplyEffectToTarget(OtherActor, EffectInfo);
		}
	}

	if (bDestroyOnEffectsApplication)
	{
		Destroy();
	}
}

/** EndOverlap Callback */
void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!IsValid(TargetASC))
	{
		return;
	}

	TArray<FActiveGameplayEffectHandle> ActiveEffectHandlesToRemove;
	for (FEffectDefinition& EffectInfo : Effects)
	{
		// Apply effect
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(OtherActor, EffectInfo);
		}

		// Remove active effects applied on OtherActor's ASC
		if (EffectInfo.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			for (const auto EffectHandlePair : ActiveEffectHandles)
			{
				if (TargetASC == EffectHandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(EffectHandlePair.Key, EffectInfo.StacksToRemove);
					ActiveEffectHandlesToRemove.Add(EffectHandlePair.Key);
				}
			}
		}
	}

	// Remove active effects from map
	for (FActiveGameplayEffectHandle& ActiveEffectHandle : ActiveEffectHandlesToRemove)
	{
		ActiveEffectHandles.FindAndRemoveChecked(ActiveEffectHandle);
	}
	
	if (bDestroyOnEffectsRemoval)
	{
		Destroy();
	}
}

#pragma endregion EFFECTS
