// Copyright Sergio Calahorro

#include "Actor/AuraEffectActor.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "GAS/Effects/EffectInfo.h"

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
FActiveGameplayEffectHandle AAuraEffectActor::ApplyEffectToTarget(AActor* Target, const TSubclassOf<UGameplayEffect>& EffectClass) const
{
	check(EffectClass);

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC)
	{
		return false;
	}

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	return ActiveEffectHandle;
}

/** Remove active effect from target Actor */
bool AAuraEffectActor::RemoveActiveEffectFromTarget(AActor* Target, const FActiveGameplayEffectHandle& ActiveEffectHandle) const
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC || !ActiveEffectHandle.WasSuccessfullyApplied())
	{
		return false;
	}
	
	return TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
}

/** BeginOverlap Callback */
void AAuraEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const FEffectInfo& EffectInfo : Effects)
	{
		// Apply on BeginOverlap
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
		{
			const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectToTarget(OtherActor, EffectInfo.EffectClass);

			// Store active effect handle if it should be removed on EndOverlap
			if (EffectInfo.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
			{
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					ActiveEffectHandles.AddUnique(ActiveGameplayEffectHandle);
				}
			}
		}
	}
}

/** EndOverlap Callback */
void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (const FEffectInfo& EffectInfo : Effects)
	{
		// Apply on EndOverlap
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectToTarget(OtherActor, EffectInfo.EffectClass);

			// Store active effect handle if it should be removed on EndOverlap
			if (EffectInfo.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
			{
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					ActiveEffectHandles.AddUnique(ActiveGameplayEffectHandle);
				}
			}
		}
	}

	// Remove active effects
	for (const FActiveGameplayEffectHandle& ActiveGameplayEffectHandle : ActiveEffectHandles)
	{
		RemoveActiveEffectFromTarget(OtherActor, ActiveGameplayEffectHandle);
	}

	ActiveEffectHandles.Empty();

	if (bDestroyOnEffectsRemoval)
	{
		Destroy();
	}
}

#pragma endregion EFFECTS
