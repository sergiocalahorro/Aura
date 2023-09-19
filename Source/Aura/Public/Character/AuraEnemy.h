// Copyright Sergio Calahorro

#pragma once

#include "CoreMinimal.h"
#include "AuraBaseCharacter.h"
#include "AuraEnemy.generated.h"

UCLASS(Abstract)
class AURA_API AAuraEnemy : public AAuraBaseCharacter
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
	
};
