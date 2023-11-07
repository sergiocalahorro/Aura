// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

// Forward declarations - Aura
class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedSignature, const FGameplayTag&, const FGameplayTag&, int32);
DECLARE_DELEGATE_OneParam(FBroadcastAbilitySignature, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquippedSignature, const FGameplayTag&, const FGameplayTag&, const FGameplayTag&, const FGameplayTag&);

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:

	/** RepNotify callback for ActivatableAbilities */
	virtual void OnRep_ActivateAbilities() override;

#pragma endregion OVERRIDES

#pragma region CORE

public:

	/** Function called once AbilityActorInfo has been set */
	void AbilityActorInfoSet();
	
#pragma endregion CORE

#pragma region EFFECTS

public:

	/** Delegate called when an effect is applied, to broadcast its asset tags */
	FEffectAssetTagsSignature EffectAssetTagsDelegate;
	
protected:

	/** Callback called when a Gameplay Effect is applied */
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
	
#pragma endregion EFFECTS

#pragma region ABILITIES

public:

	/** Add given list of abilities */
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	/** Add given list of passive abilities */
	void AddPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	/** Activate ability by input tag when released */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/** Activate ability by input tag when held */
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	/** Broadcast ability */
	void BroadcastAbility(const FBroadcastAbilitySignature& BroadcastAbilityDelegate);

	/** Update abilities' statuses based on level */
	void UpdateAbilitiesStatuses(int32 Level);

	/** Spend spell point in order to upgrade an ability */
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	/** Equip ability */
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	/** Clear an ability's input tag from its spec */
	void ClearInputTagFromSpec(FGameplayAbilitySpec* AbilitySpec);

	/** Clear any abilities assigned to this input tag */
	void ClearAbilitiesOfInputTag(const FGameplayTag& InputTag);

	/** Get ability's tag from ability spec */
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/** Get ability's input tag from ability spec */
	static FGameplayTag GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	/** Get ability's input tag from ability tag */
	FGameplayTag GetAbilityInputTagFromAbilityTag(const FGameplayTag& AbilityTag);

	/** Get ability's status tag from ability spec */
	static FGameplayTag GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	/** Get ability's status tag from ability tag */
	FGameplayTag GetAbilityStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);

	/** Get ability spec from ability's tag */
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	/** Check whether the ability has a given input tag */
	bool AbilityHasInputTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& InputTag) const;

	/** Get ability's descriptions by its tag */
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

protected:

	/** Client RPC called to equip an ability */
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);

	/** Client RPC called to update an ability's status */
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);

public:

	/** Whether startup abilities have been given */
	bool bStartupAbilitiesGiven;

	/** Delegate called when abilities are given */
	FAbilitiesGivenSignature AbilitiesGivenDelegate;

	/** Delegate called when an ability status has changed */
	FAbilityStatusChangedSignature AbilityStatusChangedDelegate;

	/** Delegate called when an ability has been equipped */
	FAbilityEquippedSignature AbilityEquippedDelegate;

#pragma endregion ABILITIES

#pragma region ATTRIBUTES

public:
	
	/** Upgrade attribute with given tag */
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
private:

	/** (Server) Upgrade attribute with given tag */
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
#pragma endregion ATTRIBUTES

};
