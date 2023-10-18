// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"

#include "AnimNotify_SendGameplayEventTag.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAnimNotify_SendGameplayEventTag : public UAnimNotify
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	UAnimNotify_SendGameplayEventTag(const FObjectInitializer& ObjectInitializer);

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Function called when the Notify is reached */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,	const FAnimNotifyEventReference& EventReference) override;

	/** Implementable event to get a custom name for the notify */
	FString GetNotifyName_Implementation() const;
	
#pragma endregion OVERRIDES

#pragma region EVENT

private:

	/** Event's GameplayTag sent to the owning Actor when this Notify is reached */
	UPROPERTY(EditAnywhere, Category = "AA|Event")
	FGameplayTag EventTag;

#pragma endregion EVENT
};
