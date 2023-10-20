// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"

#include "BTTask_Attack_Elementalist.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTTask_Attack_Elementalist : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UBTTask_Attack_Elementalist();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

private:

	/** Execute Task */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** Abort Task */
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

#pragma endregion OVERRIDES

#pragma region TASK

private:

	/** Combat target key selector */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CombatTargetSelector;

	/** Attack tag used to trigger ability associated with it */
	UPROPERTY(EditAnywhere)
	FGameplayTag AttackTag;

	/** Summon tag used to trigger ability associated with it */
	UPROPERTY(EditAnywhere)
	FGameplayTag SummonTag;

	/** Threshold used to decide what ability to use. If the minion count is above this threshold, an attack will be performed */
	UPROPERTY(EditAnywhere)
	int32 MinionSummonThreshold = 2;
};
