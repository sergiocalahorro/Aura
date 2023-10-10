// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:

	/** Get level */
	virtual int32 GetCurrentLevel() const;

	/** Get socket's location that will be used in combat */
	virtual FVector GetCombatSocketLocation() const;

	/** Set target location to face */
	virtual void SetFacingTarget(const FVector& FacingTargetLocation);

	/** Get HitReact's montage */
	virtual UAnimMontage* GetHitReactMontage() const;
};
