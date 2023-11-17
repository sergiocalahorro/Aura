// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

// Headers - Aura
#include "Interaction/CombatInterface.h"
#include "Interaction/AttackData.h"

#include "AuraBaseCharacter.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UMotionWarpingComponent;
class UNiagaraSystem;
struct FActiveGameplayEffectHandle;

// Forward declarations - Aura
struct FEffectDefinition;

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

public:

	/** Returns the properties used for network replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	/** Get character's class */
	virtual ECharacterClass GetCharacterClass_Implementation() override;

	/** Get Avatar that is the owner of the interface */
	virtual AActor* GetAvatar_Implementation() override;
	
	/** Get socket's location that will be used in combat */
	virtual FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const override;

	/** Get weapon */
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;

	/** Get socket's location for the weapon */
	virtual FVector GetWeaponSocketLocation_Implementation(const FName& SocketName) const override;

	/** Set target location to face */
	virtual void SetFacingTarget(const FVector& FacingTargetLocation) override;

	/** Get all attacks */
	TArray<FAttackData> GetAllAttacks_Implementation() const;

	/** Get attacks of given type */
	TArray<FAttackData> GetAttacksOfType_Implementation(EAttackType AttackType) const;

	/** Get attack with given tag */
	FAttackData GetAttackWithTag_Implementation(const FGameplayTag& MontageTag) const;

	/** Get HitReact's montage */
	virtual UAnimMontage* GetHitReactMontage() const override;

	/** Apply knockback */
	virtual void Knockback_Implementation(const FVector& KnockbackForce) override;

	/** Functionality performed on death */
	virtual void Death(const FVector& DeathImpulse) override;

	/** Whether is dead */
	virtual bool IsDead_Implementation() const override;

	/** Handle death */
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	/** Dissolve effect */
	void Dissolve();

	/** Start timeline to apply dissolve effect */
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMaterialInstances);

	/** Get blood particles' effect */
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;

	/** Get minion count */
	int32 GetMinionCount_Implementation() const;

	/** Modify minion count by amount */
	void ModifyMinionCount_Implementation(int32 Amount);

	/** Get delegate that is broadcasted once the ASC has been registered */
	virtual FASCRegisteredSignature& GetASCRegisteredDelegate() override;

	/** Get delegate that is broadcasted once the actor has died */
	virtual FDeathSignature& GetDeathDelegate() override;

	/** Set whether is being shocked */
	virtual void SetIsBeingShocked_Implementation(bool bInIsBeingShocked) override;

	/** Get whether is being shocked */
	virtual bool IsBeingShocked_Implementation() const override;

	/** Getter for bIsStunned */
	bool IsStunned() const { return bIsStunned; }

protected:

	/** Callback called whenever Stun's tag is changed */
	virtual void StunTagChanged(const FGameplayTag GameplayTag, int32 NewCount);

	/** RepNotify callback for bIsStunned */
	UFUNCTION()
	virtual void OnRep_IsStunned();

protected:
		
	/** Character's class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Core")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	/** Base value for walking speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Core", meta = (ClampMin = 0.f, UIMin = 0.f, Delta = 1.f, ForceUnits = "cm/s"))
	float BaseWalkSpeed = 600.f;

	/** Weapon mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Combat|Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** Attack sockets tagged */
	UPROPERTY(EditAnywhere, Category = "AA|Combat|Weapon")
	TMap<FGameplayTag, FName> AttackSockets;

	/** Attack montages */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Attacks")
	TArray<FAttackData> Attacks;
		
	/** HitReact's montage to play */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|HitReact")
	TObjectPtr<UAnimMontage> HitReactMontage;

	/** Whether character is hit reacting */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Combat|HitReact", meta = (AllowPrivateAccess = true))
	bool bHitReacting;
	
	/** Whether character is dead */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Combat|Death", meta = (AllowPrivateAccess = true))
	bool bIsDead;

	/** Whether character is stunned */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsStunned, Category = "AA|Combat|Stun", meta = (AllowPrivateAccess = true))
	bool bIsStunned;

	/** Whether character is being shocked */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "AA|Combat|Shock", meta = (AllowPrivateAccess = true))
	bool bIsBeingShocked;

	/** Dissolve's material instance */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	/** Weapon's dissolve's material instance */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/** Blood effect */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	/** Death sound */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Combat|Effects")
	TObjectPtr<USoundBase> DeathSound;

	/** Minion count */
	int32 MinionCount;

	/** Delegate broadcasted once the ASC has been registered */
	FASCRegisteredSignature ASCRegisteredDelegate;

	/** Delegate broadcasted once the character has died */
	FDeathSignature DeathDelegate;
	
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
	virtual void AddCharacterAbilities();

protected:

	/** Default effects to apply when ability system is initialized */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<FEffectDefinition> DefaultEffects;

	/** Abilities granted at startup */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** Passive abilities granted at startup */
	UPROPERTY(EditAnywhere, Category = "AA|GAS|Default")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	/** Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/** Attribute set */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

#pragma endregion GAS
	
};
