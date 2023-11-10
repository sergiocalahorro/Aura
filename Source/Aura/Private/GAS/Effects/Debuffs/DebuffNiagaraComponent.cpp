// Copyright Sergio Calahorro

#include "GAS/Effects/Debuffs/DebuffNiagaraComponent.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this object's properties */
UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (OwnerASC)
	{
		OwnerASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::OnDebuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetASCRegisteredDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::OnDebuffTagChanged);
			}
		);
	}

	if (CombatInterface)
	{
		CombatInterface->GetDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

#pragma endregion OVERRIDES

#pragma region DEBUFF

/** Callback called whenever there's a change on the debuff tag */
void UDebuffNiagaraComponent::OnDebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0 && IsValid(GetOwner()) && GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner()))
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

/** Callback called when the owner is dead */
void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}

#pragma endregion DEBUFF