// Copyright Sergio Calahorro

#include "Animation/AnimNotifies/AnimNotify_SendGameplayEventTag.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"

#pragma region INITIALIZATION

/** Constructor */
UAnimNotify_SendGameplayEventTag::UAnimNotify_SendGameplayEventTag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 255, 0, 255);
#endif // WITH_EDITORONLY_DATA

	bIsNativeBranchingPoint = false;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Function called when the Notify is reached */
void UAnimNotify_SendGameplayEventTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());
	}
}

/** Implementable event to get a custom name for the notify */
FString UAnimNotify_SendGameplayEventTag::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		return EventTag.ToString();
	}

	return Super::GetNotifyName_Implementation();
}

#pragma endregion OVERRIDES