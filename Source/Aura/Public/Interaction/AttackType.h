// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

#include "AttackType.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Melee,
	Ranged
};
