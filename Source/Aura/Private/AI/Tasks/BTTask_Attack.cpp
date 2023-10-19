// Copyright Sergio Calahorro

#include "AI/Tasks/BTTask_Attack.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BlueprintGameplayTagLibrary.h"

// Headers - Aura
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interaction/CombatInterface.h"

#pragma region OVERRIDES

/** Execute Task */
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (UAbilitySystemComponent* PawnASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn))
	{
		AActor* CombatTarget = UBTFunctionLibrary::GetBlackboardValueAsActor(this, CombatTargetSelector);
		ICombatInterface::Execute_SetCombatTarget(ControlledPawn, CombatTarget);

		const FGameplayTagContainer AttackTagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(AttackTag);
		PawnASC->TryActivateAbilitiesByTag(AttackTagContainer);

		FinishExecute(true);
		return EBTNodeResult::Type::Succeeded;
	}
	

	FinishExecute(false);
	return EBTNodeResult::Type::Failed;
}

/** Abort Task */
EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

#pragma endregion OVERRIDES