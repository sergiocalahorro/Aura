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
DECLARE_DELEGATE_OneParam(FBroadcastAbilitySignature, const FGameplayAbilitySpec&);

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

	/** Delegate called when abilities are given */
	FAbilitiesGivenSignature AbilitiesGivenDelegate;
	
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

	/** Get ability's tag from ability spec */
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/** Get ability's input tag from ability spec */
	static FGameplayTag GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/** Get ability's status tag from ability spec */
	static FGameplayTag GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/** Upgrade attribute with given tag */
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

private:

	/** (Server) Upgrade attribute with given tag */
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

public:

	/** Whether startup abilities have been given */
	bool bStartupAbilitiesGiven;

#pragma endregion ABILITIES

};
