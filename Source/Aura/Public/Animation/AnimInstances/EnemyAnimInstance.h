// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "Animation/AnimInstances/AuraBaseAnimInstance.h"

#include "EnemyAnimInstance.generated.h"

// Forward declarations - Aura
class AAuraEnemy;

/**
 * 
 */
UCLASS()
class AURA_API UEnemyAnimInstance : public UAuraBaseAnimInstance
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

	/** Enemy's reference */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core")
	TObjectPtr<AAuraEnemy> Enemy;

#pragma endregion CORE
	
};
