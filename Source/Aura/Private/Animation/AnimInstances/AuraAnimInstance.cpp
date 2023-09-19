// Copyright Sergio Calahorro

#include "Animation/AnimInstances/AuraAnimInstance.h"

// Headers - Aura
#include "Character/AuraCharacter.h"

#pragma region OVERRIDES
	
/** Native initialization override point */
void UAuraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (IsValid(Character))
	{
		Aura = Cast<AAuraCharacter>(Character);
	}
}
	
/** Native update override point. It is usually a good idea to simply gather data in this step and for the bulk of the work to be done in NativeThreadSafeUpdateAnimation */
void UAuraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

/** Native thread safe update override point. Executed on a worker thread just prior to graph update for linked anim instances, only called when the hosting node(s) are relevant */
void UAuraAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) 
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

#pragma endregion OVERRIDES