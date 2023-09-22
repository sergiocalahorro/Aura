// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "EffectApplicationPolicy.h"
#include "EffectRemovalPolicy.h"

#include "EffectDefinition.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
struct FActiveGameplayEffectHandle;

USTRUCT()
struct FEffectDefinition
{
	GENERATED_BODY()
	
public:

	/** Effect to be applied */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectClass;

	/** Effect's level */
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f, ClampMax = 10.f, UIMax = 10.f, Delta = 1.f))
	float EffectLevel = 1.f;

	/** Policy used for applying the effect */
	UPROPERTY(EditAnywhere)
	EEffectApplicationPolicy EffectApplicationPolicy;

	/** Policy used for removing the effect */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EffectApplicationPolicy != EEffectApplicationPolicy::DoNotApply", EditConditionHides))
	EEffectRemovalPolicy EffectRemovalPolicy;

	/** Effect's stacks to remove (if the value is set to -1, then all stacks will be removed) */
	UPROPERTY(EditAnywhere, meta = (ClampMin = -1, UIMin = -1, ClampMax = 1, UIMax = 1, EditCondition = "EffectApplicationPolicy != EEffectApplicationPolicy::DoNotApply", EditConditionHides))
	int32 StacksToRemove = 1;
};
