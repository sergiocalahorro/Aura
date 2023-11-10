// Copyright Sergio Calahorro

#include "GAS/Cues/CueDebuff.h"

// Headers - Unreal Engine
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Headers - Aura
#include "Interaction/CombatInterface.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
ACueDebuff::ACueDebuff()
{
	bAutoDestroyOnRemove = false;
	bAllowMultipleOnActiveEvents = false;
	bAllowMultipleWhileActiveEvents = false;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when a GameplayCue with duration is first activated, this will only be called if the client witnessed the activation */
bool ACueDebuff::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (!DebuffComponent)
	{
		DebuffComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(DebuffSystem, MyTarget->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false, true);

		if (DebuffComponent)
		{
			DebuffComponent->OnSystemFinished.AddUniqueDynamic(this, &ACueDebuff::OnDebuffSystemFinished);
			
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MyTarget))
			{
				CombatInterface->GetDeathDelegate().AddUniqueDynamic(this, &ACueDebuff::OnTargetDestroyed);
			}
		}
	}
	
	return Super::OnActive_Implementation(MyTarget, Parameters);
}

/** Called when a GameplayCue with duration is removed */
bool ACueDebuff::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (DebuffComponent)
	{
		DebuffComponent->Deactivate();
	}
	
	return Super::OnRemove_Implementation(MyTarget, Parameters);
}

#pragma endregion OVERRIDES

#pragma region DEBUFF

/** Called when target is destroyed */
void ACueDebuff::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (DebuffComponent)
	{
		DebuffComponent->Deactivate();
	}
}

/** Callback called when the debuff's particle system is finished */
void ACueDebuff::OnDebuffSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

#pragma endregion DEBUFF
