// Copyright Sergio Calahorro

#include "Character/AuraBaseCharacter.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Headers - Aura
#include "GAS/Effects/EffectDefinition.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraBaseCharacter::AAuraBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Setup weapon
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when the game starts or when spawned */
void AAuraBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region GAS

/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
UAbilitySystemComponent* AAuraBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/** Returns the attribute set system component to use for this actor */
UAttributeSet* AAuraBaseCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

/** Apply given effect to itself */
FActiveGameplayEffectHandle AAuraBaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float Level) const
{
	check(AbilitySystemComponent);
	
	const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	return ActiveEffectHandle;
}

/** Apply given effect definitions to itself */
void AAuraBaseCharacter::ApplyEffectDefinitionsToSelf(const TArray<FEffectDefinition>& Effects) const
{
	for (const FEffectDefinition& EffectDefinition : Effects)
	{
		if (EffectDefinition.EffectApplicationPolicy != EEffectApplicationPolicy::ApplyDefault)
		{
			continue;
		}

		ApplyEffectToSelf(EffectDefinition.EffectClass, EffectDefinition.EffectLevel);
	}
}

#pragma endregion GAS
