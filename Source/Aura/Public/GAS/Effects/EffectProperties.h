// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "EffectProperties.generated.h"

// Forward declarations - Unreal Engine
class UAbilitySystemComponent;
class ACharacter;
struct FGameplayEffectContextHandle;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

public:

	FEffectProperties()
	{
		
	}
	
public:

	/** Source's Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	/** Source's Avatar Actor */
	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor;

	/** Source's Controller */
	UPROPERTY()
	TObjectPtr<AController> SourceController;

	/** Source's Character */
	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter;

	/** Target's Ability system component */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	/** Target's Avatar Actor */
	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor;

	/** Target's Controller */
	UPROPERTY()
	TObjectPtr<AController> TargetController;

	/** Target's Character */
	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter;
	
	/** Effect context handle */
	FGameplayEffectContextHandle EffectContextHandle;
};
