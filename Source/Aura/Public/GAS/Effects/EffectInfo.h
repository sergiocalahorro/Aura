// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "EffectApplicationPolicy.h"
#include "EffectRemovalPolicy.h"

#include "EffectInfo.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;

USTRUCT()
struct FEffectInfo
{
	
public:

	/** Effect to be applied */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectClass;

	/** Policy used for applying the effect */
	UPROPERTY(EditAnywhere)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	/** Policy used for removing the effect */
	UPROPERTY(EditAnywhere)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
};

