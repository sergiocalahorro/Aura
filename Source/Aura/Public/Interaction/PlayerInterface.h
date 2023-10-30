// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:

	/** Add incoming XP to accumulated XP */
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	/** Get current accumulated XP */
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	/** Get attribute points reward for new level */
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	/** Get spell points reward for new level */
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	/** Add incoming level to player's current level */
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	/** Add incoming attribute points to player's current attribute points */
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	/** Add incoming spell points to player's current spell points */
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	/** Get player's attribute points */
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	/** Get player's spell points */
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;
	
	/** Handle level up */
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	
	/** Find level for incoming XP */
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;
};
