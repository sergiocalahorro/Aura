// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "Animation/AnimInstances/AuraBaseAnimInstance.h"

#include "AuraAnimInstance.generated.h"

// Forward declarations - Aura
class AAuraCharacter;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAnimInstance : public UAuraBaseAnimInstance
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

	/** Aura's reference */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<AAuraCharacter> Aura;

#pragma endregion CORE
	
};
