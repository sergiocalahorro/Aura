// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "AIController.h"

#include "AuraAIController.generated.h"

// Forward declarations - Unreal Engine
class UBehaviorTreeComponent;

UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this actor's properties */
	AAuraAIController();
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Starts executing behavior tree */
	virtual bool RunBehaviorTree(UBehaviorTree* BTAsset) override;

#pragma endregion OVERRIDES

#pragma region AI

protected:

	/** Behavior Tree Component */
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

#pragma endregion AI
	
};
