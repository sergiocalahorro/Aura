// Copyright Sergio Calahorro

#include "Animation/AnimNotifies/AnimNotifyMontageEvent.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"

#pragma region OVERRIDES

/** Function called when the Notify is reached */
void UAnimNotifyMontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());
	}
}

#pragma endregion OVERRIDES