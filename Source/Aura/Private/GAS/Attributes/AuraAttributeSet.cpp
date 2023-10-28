// Copyright Sergio Calahorro

#include "GAS/Attributes/AuraAttributeSet.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerController.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Primary Attributes
	TaggedAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	
	// Secondary Attributes
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	// Damage Type Resistance Attributes
	TaggedAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetResistanceFireAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetResistanceLightningAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetResistanceArcaneAttribute);
	TaggedAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetResistancePhysicalAttribute);
}
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Primary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Damage Type Resistance attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);

	// Vital attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

/** Called just before any modification happens to an attribute */
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

/**	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made */
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(EffectProperties);
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(EffectProperties);
	}
}

#pragma endregion OVERRIDES

#pragma region ATTRIBUTES

#pragma region ATTRIBUTES_PRIMARY

/** Strength's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

/** Intelligence's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

/** Resilience's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

/** Vigor's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

#pragma endregion ATTRIBUTES_PRIMARY

#pragma region ATTRIBUTES_SECONDARY

/** Armor's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

/** ArmorPenetration's Replication Notify Callback */
void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

/** BlockChance's Replication Notify Callback */
void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

/** CriticalHitChance's Replication Notify Callback */
void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

/** CriticalHitDamage's Replication Notify Callback */
void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

/** CriticalHitResistance's Replication Notify Callback */
void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

/** HealthRegeneration's Replication Notify Callback */
void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

/** ManaRegeneration's Replication Notify Callback */
void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

/** MaxHealth's Replication Notify Callback */
void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

/** MaxMana's Replication Notify Callback */
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

#pragma endregion ATTRIBUTES_SECONDARY

#pragma region ATTRIBUTES_RESISTANCE

/** ResistanceFire's Replication Notify Callback */
void UAuraAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceFire, OldResistanceFire);
}

/** ResistanceLightning's Replication Notify Callback */
void UAuraAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceLightning, OldResistanceLightning);
}

/** ResistanceArcane's Replication Notify Callback */
void UAuraAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldResistanceArcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceArcane, OldResistanceArcane);
}

/** ResistancePhysical's Replication Notify Callback */
void UAuraAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistancePhysical, OldResistancePhysical);
}

#pragma endregion ATTRIBUTES_RESISTANCE

#pragma region ATTRIBUTES_VITAL

/** Health's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

/** Mana's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

#pragma endregion ATTRIBUTES_VITAL

#pragma region ATTRIBUTES_META

/** Handle incoming damage */
void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& EffectProperties)
{
	const float IncomingDamageAmount = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (IncomingDamageAmount > 0.f)
	{
		const float NewHealth = GetHealth() - IncomingDamageAmount;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		
		const bool bOutOfHealth = NewHealth <= 0.f;
		if (bOutOfHealth)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
			{
				CombatInterface->Death();
			}
			SendXPEvent(EffectProperties);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FAuraGameplayTags::Get().Abilities_HitReact);
			EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}

		ShowFloatingDamageText(EffectProperties, IncomingDamageAmount);
	}
}

/** Show floating damage text above target */
void UAuraAttributeSet::ShowFloatingDamageText(const FEffectProperties& EffectProperties, float Damage) const
{
	if (EffectProperties.SourceCharacter == EffectProperties.TargetCharacter)
	{
		return;	
	}

	const bool bIsBlockedHit = UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
	const bool bIsCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);

	// ToDo: Create a Damage struct holding info such as the DamageAmount, its DamageType(s), whether it's a blocked/critical hit...

	if (AAuraPlayerController* SourcePlayerController = Cast<AAuraPlayerController>(EffectProperties.SourceController))
	{
		SourcePlayerController->DisplayDamage(EffectProperties.TargetCharacter, Damage, bIsBlockedHit, bIsCriticalHit);
		return;
	}

	if (AAuraPlayerController* TargetPlayerController = Cast<AAuraPlayerController>(EffectProperties.TargetController))
	{
		TargetPlayerController->DisplayDamage(EffectProperties.TargetCharacter, Damage, bIsBlockedHit, bIsCriticalHit);
	}
}

/** Handle incoming XP */
void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& EffectProperties)
{
	const float IncomingXPAmount = GetIncomingXP();
	SetIncomingXP(0.f);

	ACharacter* SourceCharacter = EffectProperties.SourceCharacter;
	if (SourceCharacter->Implements<UCombatInterface>() && SourceCharacter->Implements<UPlayerInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCurrentLevel(SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(SourceCharacter, CurrentXP + IncomingXPAmount);

		const int32 NumberOfLevelUps = NewLevel - CurrentLevel;
		if (NumberOfLevelUps > 0)
		{
			// Increase player's level and reward player with both attribute and spell points
			const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(SourceCharacter, CurrentLevel);
			const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(SourceCharacter, CurrentLevel);

			IPlayerInterface::Execute_AddToPlayerLevel(SourceCharacter, NumberOfLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(SourceCharacter, SpellPointsReward);
			IPlayerInterface::Execute_LevelUp(SourceCharacter);

			// Fill health and mana
			SetHealth(GetMaxHealth());
			SetMana(GetMaxMana());
		}

		IPlayerInterface::Execute_AddToXP(SourceCharacter, IncomingXPAmount);
	}
}

/** Send XP event */
void UAuraAttributeSet::SendXPEvent(const FEffectProperties& EffectProperties)
{
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetCurrentLevel(EffectProperties.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(EffectProperties.TargetCharacter);
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(EffectProperties.TargetCharacter, TargetClass, TargetLevel);

		const FGameplayTag XPEventTag = FAuraGameplayTags::Get().Attributes_Meta_IncomingXP;

		FGameplayEventData Payload;
		Payload.EventTag = XPEventTag;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProperties.SourceCharacter, XPEventTag, Payload);
	}
}

#pragma endregion ATTRIBUTES_META

#pragma endregion ATTRIBUTES

#pragma region EFFECTS

/** Populate structure containing the effect's properties */
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData &Data, FEffectProperties& Properties) const
{
	Properties.EffectContextHandle = Data.EffectSpec.GetContext();

	// Setup Source's properties
	Properties.SourceASC = Properties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Properties.SourceASC) && Properties.SourceASC->AbilityActorInfo.IsValid() && Properties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.SourceAvatarActor = Properties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Properties.SourceController = Properties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (!Properties.SourceController && Properties.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(Properties.SourceAvatarActor))
			{
				Properties.SourceController = Pawn->GetController();
			}
		}

		if (Properties.SourceController)
		{
			Properties.SourceCharacter = Cast<ACharacter>(Properties.SourceController->GetPawn());
		}
	}

	// Setup Target's properties
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetAvatarActor);
		Properties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.TargetAvatarActor);
	}
}

#pragma endregion EFFECTS