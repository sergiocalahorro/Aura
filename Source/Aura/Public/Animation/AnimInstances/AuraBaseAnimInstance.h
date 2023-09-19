// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "AuraBaseAnimInstance.generated.h"

// Forward declarations - Unreal Engine
class UCharacterMovementComponent;

// Forward declarations - Aura
class AAuraBaseCharacter;

/**
 * 
 */
UCLASS()
class AURA_API UAuraBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
public:

	/** Native initialization override point */
	virtual void NativeInitializeAnimation() override;
	
	/** Native update override point. It is usually a good idea to simply gather data in this step and for the bulk of the work to be done in NativeThreadSafeUpdateAnimation */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/** Native thread safe update override point. Executed on a worker thread just prior to graph update for linked anim instances, only called when the hosting node(s) are relevant */
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

#pragma endregion OVERRIDES

#pragma region CORE

protected:

	/** Character's reference */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<AAuraBaseCharacter> Character;

	/** Character CharacterMovement's reference */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	/** Character's ground speed */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	float GroundSpeed;

#pragma endregion CORE
	
};
