// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IInteractableInterface
{
	GENERATED_BODY()

public:

	/** Functionality performed when Actor should be highlighted (on hovered) */
	virtual void HighlightActor() = 0;

	/** Functionality performed when Actor should be un-highlighted (stopped being hovered) */
	virtual void UnHighlightActor() = 0;
};
