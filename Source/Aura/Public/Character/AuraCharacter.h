// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AuraBaseCharacter.h"

#include "AuraCharacter.generated.h"

// Forward declarations - Unreal Engine
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacter : public AAuraBaseCharacter
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this character's properties */
	AAuraCharacter();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

private:

	/** Camera */
	UPROPERTY(EditAnywhere, Category = "AA|Components")
	TObjectPtr<UCameraComponent> Camera;
	
	/** Spring Arm */
	UPROPERTY(EditAnywhere, Category = "AA|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

#pragma endregion COMPONENTS
	
};
