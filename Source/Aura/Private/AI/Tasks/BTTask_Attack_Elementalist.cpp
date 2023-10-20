// Copyright Sergio Calahorro

#include "AI/Tasks/BTTask_Attack_Elementalist.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BlueprintGameplayTagLibrary.h"
#include "BehaviorTree/BTFunctionLibrary.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UBTTask_Attack_Elementalist::UBTTask_Attack_Elementalist()
{
	AttackTag = FAuraGameplayTags::Get().Abilities_Attack;
	SummonTag = FAuraGameplayTags::Get().Abilities_Summon;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Execute Task */
EBTNodeResult::Type UBTTask_Attack_Elementalist::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (UAbilitySystemComponent* PawnASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn))
	{
		AActor* CombatTarget = UBTFunctionLibrary::GetBlackboardValueAsActor(this, CombatTargetSelector);
		ICombatInterface::Execute_SetCombatTarget(ControlledPawn, CombatTarget);
		const int32 MinionCount = ICombatInterface::Execute_GetMinionCount(ControlledPawn);
		const FGameplayTag AbilityTag = MinionCount < MinionSummonThreshold ? SummonTag : AttackTag;
		const FGameplayTagContainer AbilityTagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(AbilityTag);
		PawnASC->TryActivateAbilitiesByTag(AbilityTagContainer);
		
		FinishExecute(true);
		return EBTNodeResult::Type::Succeeded;
	}
	
	FinishExecute(false);
	return EBTNodeResult::Type::Failed;
}

/** Abort Task */
EBTNodeResult::Type UBTTask_Attack_Elementalist::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

#pragma endregion OVERRIDES