// Copyright Sergio Calahorro

#include "Animation/AnimInstances/AuraBaseAnimInstance.h"

// Headers - Unreal Engine
#include "GameFramework/CharacterMovementComponent.h"

// Headers - Aura
#include "Character/AuraBaseCharacter.h"

#pragma region OVERRIDES
	
/** Native initialization override point */
void UAuraBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (APawn* PawnOwner = TryGetPawnOwner())
	{
		Character = Cast<AAuraBaseCharacter>(PawnOwner);
		CharacterMovement = Character->GetCharacterMovement();
	}
}
	
/** Native update override point. It is usually a good idea to simply gather data in this step and for the bulk of the work to be done in NativeThreadSafeUpdateAnimation */
void UAuraBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovement)
	{
		GroundSpeed = CharacterMovement->Velocity.Size2D();
		bShouldMove = GroundSpeed > IdleSpeed;
		bIsFalling = CharacterMovement->IsFalling();
	}
}

/** Native thread safe update override point. Executed on a worker thread just prior to graph update for linked anim instances, only called when the hosting node(s) are relevant */
void UAuraBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) 
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

#pragma endregion OVERRIDES