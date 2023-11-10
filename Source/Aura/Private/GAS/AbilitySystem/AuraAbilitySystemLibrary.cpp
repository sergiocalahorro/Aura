﻿// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"

// Headers - Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Headers - Aura
#include "Character/Data/CharacterClassDefaultInfo.h"
#include "Character/Data/CharacterClassInfo.h"
#include "GameMode/AuraBaseGameMode.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/Utils/AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/WidgetControllerParams.h"

#pragma region WIDGET

/** Get Overlay Widget Controller */
UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

/** Get Attribute Menu Widget Controller */
UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

/** Get Spell Menu Widget Controller */
USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

/** Create widget controller params */
bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			OutWidgetControllerParams.AttributeSet = AttributeSet;
			OutWidgetControllerParams.AbilitySystemComponent = AbilitySystemComponent;
			OutWidgetControllerParams.PlayerState = PlayerState;
			OutWidgetControllerParams.PlayerController = PlayerController;

			return true;
		}
	}

	return false;
}

#pragma endregion WIDGET

#pragma region CHARACTER

/** Initialize character with its default attributes' values per level */
void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	check(CharacterClassInfo);
	
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	ApplyEffectToSelf(AbilitySystemComponent, ClassDefaultInfo.PrimaryAttributes, Level);
	ApplyEffectToSelf(AbilitySystemComponent, CharacterClassInfo->SecondaryAttributes, Level);
	ApplyEffectToSelf(AbilitySystemComponent, CharacterClassInfo->VitalAttributes, Level);
}

/** Initialize character with its default abilities */
void UAuraAbilitySystemLibrary::GiveDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass)
{
	check(AbilitySystemComponent);
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	check(CharacterClassInfo);

	AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
	const int32 AbilityLevel = AvatarActor->Implements<UCombatInterface>() ? ICombatInterface::Execute_GetCurrentLevel(AvatarActor) : 1;

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, AbilityLevel));
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultInfo.StartupAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, AbilityLevel));
	}
}

/** Get DataAsset containing information for character class */
UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraBaseGameMode* AuraGameMode = Cast<AAuraBaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	return AuraGameMode ? AuraGameMode->CharacterClassInfo : nullptr;
}

/** Get XP reward for given class and level */
int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo)
	{
		return 0;
	}

	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = CharacterClassDefaultInfo.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

#pragma endregion CHARACTER

#pragma region ABILITIES

/** Get DataAsset containing information for abilities */
UAbilitiesInfo* UAuraAbilitySystemLibrary::GetAbilitiesInfo(const UObject* WorldContextObject)
{
	const AAuraBaseGameMode* AuraGameMode = Cast<AAuraBaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	return AuraGameMode ? AuraGameMode->AbilitiesInfo : nullptr;
}

#pragma endregion ABILITIES

#pragma region EFFECTS

/** Apply effect to self */
FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyEffectToSelf(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel)
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, EffectLevel, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	return EffectContextHandle;
}

/** Apply damage effect to target */
FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(DamageEffectParams.SourceASC->GetAvatarActor());

	SetDamageType(EffectContextHandle, DamageEffectParams.DamageType);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	const FGameplayEffectSpecHandle EffectSpecHandle = DamageEffectParams.SourceASC->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	return EffectContextHandle;
}

/** Return whether damage effect is blocked */
bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsBlockedHit();
	}

	return false;
}

/** Return whether damage effect is critical hit */
bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsCriticalHit();
	}

	return false;
}

/** Return whether damage effect can apply debuff successfully */
bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsSuccessfulDebuff();
	}

	return false;
}

/** Return debuff's damage */
float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDamage();
	}

	return 0.f;
}

/** Return debuff's duration */
float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDuration();
	}

	return 0.f;
}

/** Return debuff's frequency */
float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffFrequency();
	}

	return 0.f;
}

/** Get the effect context's damage type */
FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (EffectContext->GetDamageType().IsValid())
		{
			return *EffectContext->GetDamageType();	
		}
	}

	return FGameplayTag();
}

/** Get the effect context's death impulse */
FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDeathImpulse();
	}

	return FVector::ZeroVector;
}

/** Get the effect context's knockback force */
FVector UAuraAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* EffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetKnockbackForce();
	}

	return FVector::ZeroVector;
}

/** Set whether damage effect is blocked */
void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

/** Set whether damage effect is critical hit */
void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

/** Set whether damage effect can apply debuff successfully */
void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

/** Set debuff's damage */
void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

/** Set debuff's duration */
void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

/** Set debuff's frequency */
void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

/** Set damage type */
void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		EffectContext->SetDamageType(DamageType);
	}
}

/** Set death impulse */
void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

/** Set knockback force */
void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce)
{
	if (FAuraGameplayEffectContext* EffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

#pragma endregion EFFECTS

#pragma region UTILS

/** Initialize character with its default abilities */
void UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(const UObject* WorldContextObject, const FVector& Origin, float Radius, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutAliveCharacters)
{	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, Origin, Radius, ObjectTypes, AActor::StaticClass(), ActorsToIgnore, OverlappedActors);
	
	for (AActor* OverlappedActor : OverlappedActors)
	{
		if (!IsValid(OverlappedActor))
		{
			continue;
		}
		
		if (OverlappedActor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlappedActor))
		{
			OutAliveCharacters.AddUnique(OverlappedActor);
		}
	}
}

/** Check whether an Actor can damage another Actor based on their tags */
bool UAuraAbilitySystemLibrary::AreActorsFriends(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bAreBothPlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bAreBothEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	return bAreBothPlayers || bAreBothEnemies;
}

#pragma endregion UTILS