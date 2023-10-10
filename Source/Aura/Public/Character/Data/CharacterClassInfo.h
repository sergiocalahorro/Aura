// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Headers - Aura
#include "Character/Data/CharacterClass.h"
#include "Character/Data/CharacterClassDefaultInfo.h"

#include "CharacterClassInfo.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Get the default information for the given class */
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

public:

	/** Default class information */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	/** Default values for Secondary Attributes */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	/** Default values for Vital Attributes */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	/** Default Abilities */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Defaults")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};
