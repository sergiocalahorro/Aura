// Copyright Sergio Calahorro

#include "GAS/Attributes/AuraAttributeSet.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(25.f);
	InitMaxMana(50.f);
}
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
}

#pragma endregion OVERRIDES

#pragma region ATTRIBUTES

/** Health's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

/** MaxHealth's Replication Notify Callback */
void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

/** Mana's Replication Notify Callback */
void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

/** MaxMana's Replication Notify Callback */
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

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