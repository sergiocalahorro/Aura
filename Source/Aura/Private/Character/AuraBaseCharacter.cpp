// Copyright Sergio Calahorro

#include "Character/AuraBaseCharacter.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"

// Headers - Aura
#include "Aura.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Effects/EffectDefinition.h"
#include "GAS/Effects/Debuffs/DebuffNiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraBaseCharacter::AAuraBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("BurnDebuffComponent"));
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;

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

/** Get character's class */
ECharacterClass AAuraBaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

/** Get Avatar that is the owner of the interface */
AActor* AAuraBaseCharacter::GetAvatar_Implementation()
{
	return this;
}

/** Get socket's location that will be used in combat */
FVector AAuraBaseCharacter::GetCombatSocketLocation(const FGameplayTag& MontageTag) const
{
	if (IsValid(Weapon) && MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Weapon))
	{
		return Weapon->GetSocketLocation(*AttackSockets.Find(MontageTag));
	}

	return GetMesh()->GetSocketLocation(*AttackSockets.Find(MontageTag));
}

/** Set target to face */
void AAuraBaseCharacter::SetFacingTarget(const FVector& FacingTargetLocation)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("FacingTarget", FacingTargetLocation);
}

/** Get attack montages */
TArray<FAttackData> AAuraBaseCharacter::GetAllAttacks_Implementation() const
{
	return Attacks;
}

/** Get attacks of given type */
TArray<FAttackData> AAuraBaseCharacter::GetAttacksOfType_Implementation(EAttackType AttackType) const
{
	TArray<FAttackData> AttacksOfType;
	
	for (const FAttackData& AttackData : Attacks)
	{
		if (AttackData.AttackType == AttackType)
		{
			AttacksOfType.Add(AttackData);
		}
	}
	
	return AttacksOfType;
}

/** Get attack montage with given tag */
FAttackData AAuraBaseCharacter::GetAttackWithTag_Implementation(const FGameplayTag& MontageTag) const
{
	for (const FAttackData& AttackMontage : Attacks)
	{
		if (AttackMontage.AttackMontageTag.MatchesTagExact(MontageTag))
		{
			return AttackMontage;
		}
	}

	return FAttackData();
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
	DeathDelegate.Broadcast(this);
}

/** Whether is dead */
bool AAuraBaseCharacter::IsDead_Implementation() const
{
	return bIsDead;
}

/** Handle death */
void AAuraBaseCharacter::MulticastHandleDeath_Implementation()
{
	bIsDead = true;

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
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

/** Get blood particles' effect */
UNiagaraSystem* AAuraBaseCharacter::GetBloodEffect_Implementation() const
{
	return BloodEffect;
}

/** Get minion count */
int32 AAuraBaseCharacter::GetMinionCount_Implementation() const
{
	return MinionCount;
}

/** Modify minion count by amount */
void AAuraBaseCharacter::ModifyMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

/** Get delegate that is broadcasted once the ASC has been registered */
FASCRegisteredSignature AAuraBaseCharacter::GetASCRegisteredDelegate()
{
	return ASCRegisteredDelegate;
}

/** Get delegate that is broadcasted once the actor has died */
FDeathSignature AAuraBaseCharacter::GetDeathDelegate()
{
	return DeathDelegate;
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
void AAuraBaseCharacter::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AddAbilities(StartupAbilities);
	AuraASC->AddPassiveAbilities(StartupPassiveAbilities);
}

#pragma endregion GAS
