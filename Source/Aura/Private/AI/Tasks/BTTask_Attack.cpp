// Copyright Sergio Calahorro

#include "AI/Tasks/BTTask_Attack.h"

// Headers - Unreal Engine
#include "AIController.h"

#pragma region OVERRIDES

/** Execute Task */
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	DrawDebugSphere(GetWorld(), ControlledPawn->GetActorLocation(), 40.f, 12, FColor::Red, false, 0.5f, 0);
	FinishExecute(true);
	
	return EBTNodeResult::Type::Succeeded;
}

/** Abort Task */
EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

#pragma endregion OVERRIDES