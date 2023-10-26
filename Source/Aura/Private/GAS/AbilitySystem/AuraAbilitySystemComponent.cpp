// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"

// Headers - Aura
#include "AuraLogChannels.h"
#include "GAS/Abilities/AuraGameplayAbility.h"

#pragma region OVERRIDES

/** RepNotify callback for ActivatableAbilities */
void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}

#pragma endregion OVERRIDES

#pragma region CORE

/** Function called once AbilityActorInfo has been set */
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}
	
#pragma endregion CORE

#pragma region EFFECTS

/** Callback called when a Gameplay Effect is applied */
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTagsDelegate.Broadcast(TagContainer);
}

#pragma endregion EFFECTS

#pragma region ABILITIES

/** Add given list of abilities */
void UAuraAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
		if (const UAuraGameplayAbility* Ability = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

/** Activate ability by input tag when released */
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

/** Activate ability by input tag when held */
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

/** Broadcast ability */
void UAuraAbilitySystemComponent::BroadcastAbility(const FBroadcastAbilitySignature& BroadcastAbilityDelegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!BroadcastAbilityDelegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

/** Get ability's tag from ability spec */
FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (const TObjectPtr<UGameplayAbility> Ability = AbilitySpec.Ability)
	{
		for (FGameplayTag AbilityTag : Ability.Get()->AbilityTags)
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return AbilityTag;
			}
		}
	}

	return FGameplayTag();
}

/** Get ability's input tag from ability spec */
FGameplayTag UAuraAbilitySystemComponent::GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag AbilityInputTag : AbilitySpec.DynamicAbilityTags)
	{
		if (AbilityInputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return AbilityInputTag;
		}
	}

	return FGameplayTag();
}

#pragma endregion ABILITIES