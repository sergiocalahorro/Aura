// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"

// Headers - Aura
#include "AuraLogChannels.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/AbilitySystem/Data/AbilitiesInfo.h"
#include "Interaction/PlayerInterface.h"

#pragma region OVERRIDES

/** RepNotify callback for ActivatableAbilities */
void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
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
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

/** Add given list of passive abilities */
void UAuraAbilitySystemComponent::AddPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
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

/** Update abilities' statuses based on level */
void UAuraAbilitySystemComponent::UpdateAbilitiesStatuses(int32 Level)
{
	UAbilitiesInfo* AbilitiesInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiesInfo->AbilitiesInformation)
	{
		if (!AbilityInfo.AbilityTag.IsValid() || Level < AbilityInfo.LevelRequirement)
		{
			continue;
		}

		if (!GetSpecFromAbilityTag(AbilityInfo.AbilityTag))
		{
			const FGameplayTag NewStatusTag = FAuraGameplayTags::Get().Abilities_Status_Eligible;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.AbilityClass, 1.f);
			AbilitySpec.DynamicAbilityTags.AddTag(NewStatusTag);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(AbilityInfo.AbilityTag, NewStatusTag);
		}
	}
}

/** Client RPC called to update an ability's status */
void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag);
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

/** Get ability's status tag from ability spec */
FGameplayTag UAuraAbilitySystemComponent::GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag AbilityStatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (AbilityStatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return AbilityStatusTag;
		}
	}

	return FGameplayTag();
}

/** Get ability spec from ability's tag */
FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

#pragma endregion ABILITIES

#pragma region ATTRIBUTES

/** Upgrade attribute with given tag */
void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

/** (Server) Upgrade attribute with given tag */
void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

#pragma endregion ATTRIBUTES