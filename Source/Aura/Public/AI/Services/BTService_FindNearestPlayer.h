// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"

#include "BTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:

	/** Update next tick interval */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

#pragma endregion OVERRIDES

#pragma region SERVICE

protected:

	/** TargetToFollow Blackboard key */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	/** DistanceToTarget Blackboard key */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;

#pragma endregion SERVICE
};
