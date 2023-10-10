// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "AuraBaseGameMode.generated.h"

// Forward declarations - Aura
class UCharacterClassInfo;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API AAuraBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region CHARACTER
	
public:

	/** Characters' class info */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Character|Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

#pragma endregion CHARACTER

};
