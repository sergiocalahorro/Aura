// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"

// Headers - Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"

// Headers - Aura
#include "Character/Data/CharacterClassDefaultInfo.h"
#include "Character/Data/CharacterClassInfo.h"
#include "GameMode/AuraBaseGameMode.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/WidgetControllerParams.h"

#pragma region WIDGET

/** Get Overlay Widget Controller */
UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

/** Get Attribute Menu Widget Controller */
UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);

			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

#pragma endregion WIDGET

#pragma region CHARACTER

/** Initialize character with its default attributes' values per level */
void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);
	
	const AAuraBaseGameMode* AuraGameMode = CastChecked<AAuraBaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	check(CharacterClassInfo);
	
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

/** Initialize character with its default abilities */
void UAuraAbilitySystemLibrary::GiveDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);
	
	const AAuraBaseGameMode* AuraGameMode = CastChecked<AAuraBaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	check(CharacterClassInfo);

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->Abilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1.f));
	}
}

#pragma endregion CHARACTER