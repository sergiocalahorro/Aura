// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

// Headers - Aura
#include "Interaction/CombatInterface.h"
#include "Interaction/TaggedMontage.h"

#include "AuraBaseCharacter.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UMotionWarpingComponent;
class UNiagaraSystem;

// Forward declarations - Aura
struct FEffectDefinition;
struct FActiveGameplayEffectHandle;

UCLASS(Abstract)
class AURA_API AAuraBaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this character's properties */
	AAuraBaseCharacter();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

protected:

	/** Motion Warping Component */
	UPROPERTY(EditAnywhere, Category = "AA|Components")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

#pragma endregion COMPONENTS

#pragma region COMBAT

public:

	/** Get Avatar that is the owner of the interface */
	virtual AActor* GetAvatar_Implementation() override;

	/** Get socket's location that will be used in combat */
	virtual FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const override;

	/** Set target location to face */
	virtual void SetFacingTarget(const FVector& FacingTargetLocation) override;

	/** Get attack montages */
	TArray<FTaggedMontage> GetAttackMontages_Implementation() const;

	/** Get attack montage with given tag */
	FTaggedMontage GetAttackMontageWithTag_Implementation(const FGameplayTag& MontageTag) const;

	/** Get HitReact's montage */
	virtual UAnimMontage* GetHitReactMontage() const override;

	/** Functionality performed on death */
	virtual void Death() override;

	/** Whether is dead */
	virtual bool IsDead_Implementation() const override;

	/** Handle death */
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	/** Dissolve effect */
	void Dissolve();

	/** Start timeline to apply dissolve effect */
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMaterialInstances);

	/** Get blood particles' effect */
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;

protected:

	/** Weapon mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Combat|Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** Attack sockets tagged */
	UPROPERTY(EditAnywhere, Category = "AA|Combat|Weapon")
	TMap<FGameplayTag, FName> AttackSockets;

	/** Attack montages */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Attacks")
	TArray<FTaggedMontage> AttackMontages;
		
	/** HitReact's montage to play */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|HitReact")
	TObjectPtr<UAnimMontage> HitReactMontage;

	/** Whether character is hit reacting */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Combat|HitReact", meta = (AllowPrivateAccess = true))
	bool bHitReacting;
	
	/** Whether character is dead */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Combat|Death", meta = (AllowPrivateAccess = true))
	bool bIsDead;

	/** Dissolve's material instance */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	/** Weapon's dissolve's material instance */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/** Blood effect */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<UNiagaraSystem> BloodEffect;
	
#pragma endregion COMBAT

#pragma region GAS

public:

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the attribute set system component to use for this actor */
	UAttributeSet* GetAttributeSet() const;

protected:

	/** Initialize ability actor info */
	virtual void InitAbilityActorInfo() PURE_VIRTUAL(AAuraBaseCharacter::InitAbilityActorInfo);
	
	/** Apply given effect to itself */
	FActiveGameplayEffectHandle ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float Level = 1.f) const;

	/** Apply given effect definitions to itself */
	void ApplyEffectDefinitionsToSelf(const TArray<FEffectDefinition>& Effects) const;

	/** Grant abilities to the character */
	virtual void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

protected:

	/** Default effects to apply when ability system is initialized */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<FEffectDefinition> DefaultEffects;

	/** Abilities granted at startup */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/** Attribute set */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

#pragma endregion GAS
	
};
