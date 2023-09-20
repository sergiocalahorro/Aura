// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraBaseCharacter.h"
#include "Interaction/HighlightableInterface.h"

#include "AuraEnemy.generated.h"

UCLASS(Abstract)
class AURA_API AAuraEnemy : public AAuraBaseCharacter, public IHighlightableInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this character's properties */
	AAuraEnemy();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region HIGHLIGHTABLE

public:

	/** Functionality performed when Actor should be highlighted (on hovered) */
	virtual void HighlightActor() override;

	/** Functionality performed when Actor should be un-highlighted (stopped being hovered) */
	virtual void UnHighlightActor() override;

#pragma endregion HIGHLIGHTABLE
	
};
