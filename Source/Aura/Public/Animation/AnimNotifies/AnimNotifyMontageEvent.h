// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"

#include "AnimNotifyMontageEvent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAnimNotifyMontageEvent : public UAnimNotify
{
	GENERATED_BODY()

#pragma region OVERRIDES

public:

	/** Function called when the Notify is reached */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,	const FAnimNotifyEventReference& EventReference) override;

#pragma endregion OVERRIDES

#pragma region EVENT

private:

	/** Event's GameplayTag sent to the owning Actor when this Notify is reached */
	UPROPERTY(EditAnywhere, Category = "AA|Event")
	FGameplayTag EventTag;

#pragma endregion EVENT
};
