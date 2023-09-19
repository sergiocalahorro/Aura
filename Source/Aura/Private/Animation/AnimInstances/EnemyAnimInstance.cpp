// Copyright Sergio Calahorro

#include "Animation/AnimInstances/EnemyAnimInstance.h"

// Headers - Aura
#include "Character/AuraEnemy.h"

#pragma region OVERRIDES
	
/** Native initialization override point */
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (IsValid(Character))
	{
		Enemy = Cast<AAuraEnemy>(Character);
	}
}
	
/** Native update override point. It is usually a good idea to simply gather data in this step and for the bulk of the work to be done in NativeThreadSafeUpdateAnimation */
void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

/** Native thread safe update override point. Executed on a worker thread just prior to graph update for linked anim instances, only called when the hosting node(s) are relevant */
void UEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) 
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

#pragma endregion OVERRIDES