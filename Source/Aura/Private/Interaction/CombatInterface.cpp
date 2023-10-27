// Copyright Sergio Calahorro

#include "Interaction/CombatInterface.h"

/** Get socket's location that will be used in combat */
FVector ICombatInterface::GetCombatSocketLocation(const FGameplayTag& MontageTag) const
{
	return FVector();
}

/** Set target location to face */
void ICombatInterface::SetFacingTarget(const FVector& FacingTargetLocation)
{
	
}

/** Get HitReact's montage */
UAnimMontage* ICombatInterface::GetHitReactMontage() const
{
	return nullptr;
}