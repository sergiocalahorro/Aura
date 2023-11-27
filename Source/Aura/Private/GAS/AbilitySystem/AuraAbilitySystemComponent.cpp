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

/** Activate ability by input tag when pressed */
void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
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

	FScopedAbilityListLock ActiveScopeLock(*this);
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

/** Activate ability by input tag when released */
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
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
			ClientUpdateAbilityStatus(AbilityInfo.AbilityTag, NewStatusTag, 1);
		}
	}
}

/** Spend spell point in order to upgrade an ability */
void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
		FGameplayTag StatusTag = GetAbilityStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(AuraGameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(AuraGameplayTags.Abilities_Status_Unlocked);
			StatusTag = AuraGameplayTags.Abilities_Status_Unlocked;
		}
		else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped) ||
				 StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}

		ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

/** Equip ability */
void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevInputTag = GetAbilityInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetAbilityStatusTagFromSpec(*AbilitySpec);

		if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped) ||
			StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
		{
			// Handle activation/deactivation for passive abilities
			if (!InputTagIsEmpty(InputTag))
			{
				// There's an ability already equipped in the desired input tag's slot
				if (FGameplayAbilitySpec* AbilitySpecInInputSlot = GetSpecWithInputTag(InputTag))
				{
					// Return early if the ability to equip is the same one that was already equipped in that input tag's slot
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*AbilitySpecInInputSlot)))
					{
						ClientEquipAbility(AbilityTag, AuraGameplayTags.Abilities_Status_Equipped, InputTag, PrevInputTag);
						return;
					}

					if (IsPassiveAbility(*AbilitySpecInInputSlot))
					{
						DeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*AbilitySpecInInputSlot));
					}
					
					ClearInputTagFromSpec(AbilitySpecInInputSlot);
				}
			}

			if (!AbilityHasAnyInputTag(AbilitySpec))
			{
				// The ability to equip isn't equipped in any input tags' slots
				if (IsPassiveAbility(*AbilitySpec))
				{
					// Activate the ability in case it's passive
					TryActivateAbility(AbilitySpec->Handle);
				}
			}

			AssignInputTagToAbility(*AbilitySpec, InputTag);
			MarkAbilitySpecDirty(*AbilitySpec);
		}

		ClientEquipAbility(AbilityTag, AuraGameplayTags.Abilities_Status_Equipped, InputTag, PrevInputTag);
	}
}

/** Client RPC called to equip an ability */
void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, InputTag, PrevInputTag);
}

/** Client RPC called to update an ability's status */
void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

/** Clear an ability's input tag from its spec */
void UAuraAbilitySystemComponent::ClearInputTagFromSpec(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag& InputTag = GetAbilityInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(InputTag);
}

/** Clear any abilities assigned to this input tag */
void UAuraAbilitySystemComponent::ClearAbilitiesOfInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(&AbilitySpec, InputTag))
		{
			ClearInputTagFromSpec(&AbilitySpec);
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

/** Get ability's input tag from ability tag */
FGameplayTag UAuraAbilitySystemComponent::GetAbilityInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetAbilityInputTagFromSpec(*AbilitySpec);
	}

	return FGameplayTag();
}

/** Get whether input tag's slot is empty */
bool UAuraAbilitySystemComponent::InputTagIsEmpty(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(AbilitySpec, InputTag))
		{
			return false;
		}
	}

	return true;
}

/** Get whether the given ability has the given input tag's slot */
bool UAuraAbilitySystemComponent::AbilityHasInputTag(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTag)
{
	return AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag);
}

/** Get ability spec with the given input tag's slot */
FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(AbilitySpec, InputTag))
		{
			return &AbilitySpec;
		}
	}

	return nullptr;
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

/** Get ability's status tag from ability tag */
FGameplayTag UAuraAbilitySystemComponent::GetAbilityStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetAbilityStatusTagFromSpec(*AbilitySpec);
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

/** Check whether the ability has a given input tag's slot */
bool UAuraAbilitySystemComponent::AbilityHasInputTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& InputTag)
{
	return AbilitySpec->DynamicAbilityTags.HasTagExact(InputTag);
}

/** Check whether the ability has any input tag's slot */
bool UAuraAbilitySystemComponent::AbilityHasAnyInputTag(const FGameplayAbilitySpec* AbilitySpec)
{
	return AbilitySpec->DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

/** Assign given ability to the given input tag's slot */
void UAuraAbilitySystemComponent::AssignInputTagToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTag)
{
	ClearInputTagFromSpec(&AbilitySpec);
	AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
}

/** Get ability's descriptions by its tag */
bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability.Get()))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}

	const UAbilitiesInfo* AbilitiesInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilitiesInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

/** Get whether ability is passive */
bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	const UAbilitiesInfo* AbilitiesInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const FAuraAbilityInfo& AbilityInfo = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag);

	return AbilityInfo.TypeTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Types_Passive);
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