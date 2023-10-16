// Copyright Sergio Calahorro

#include "AI/AuraAIController.h"

// Headers - Unreal Engine
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAuraAIController::AAuraAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Starts executing behavior tree */
bool AAuraAIController::RunBehaviorTree(UBehaviorTree* BTAsset)
{
	Blackboard->InitializeBlackboard(*BTAsset->BlackboardAsset);
	return Super::RunBehaviorTree(BTAsset);
}

#pragma endregion OVERRIDES