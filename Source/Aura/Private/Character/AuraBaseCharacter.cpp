// Copyright Sergio Calahorro

#include "Character/AuraBaseCharacter.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Headers - Aura
#include "Aura.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Effects/EffectDefinition.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraBaseCharacter::AAuraBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup collisions
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

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

#pragma region COMBAT

/** Get socket's location that will be used in combat */
FVector AAuraBaseCharacter::GetCombatSocketLocation() const
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

/** Get HitReact's montage */
UAnimMontage* AAuraBaseCharacter::GetHitReactMontage() const
{
	return HitReactMontage;
}

/** Functionality performed on death */
void AAuraBaseCharacter::Death()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

/** Handle death */
void AAuraBaseCharacter::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

/** Dissolve effect */
void AAuraBaseCharacter::Dissolve()
{
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;
	
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicMaterialInstances.AddUnique(DynamicDissolveMaterialInstance);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicMaterialInstances.AddUnique(DynamicDissolveMaterialInstance);
	}

	StartDissolveTimeline(DynamicMaterialInstances);
}

#pragma endregion COMBAT

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
	
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	return ActiveEffectHandle;
}

/** Apply given effect definitions to itself */
void AAuraBaseCharacter::ApplyEffectDefinitionsToSelf(const TArray<FEffectDefinition>& Effects) const
{
	if (Effects.IsEmpty())
	{
		return;
	}
	
	for (const FEffectDefinition& EffectDefinition : Effects)
	{
		if (EffectDefinition.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyDefault)
		{
			ApplyEffectToSelf(EffectDefinition.EffectClass, EffectDefinition.EffectLevel);
		}
	}
}

/** Grant abilities to the character */
void AAuraBaseCharacter::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	if (!HasAuthority())
	{
		return;
	}

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AddAbilities(Abilities);
}

#pragma endregion GAS
