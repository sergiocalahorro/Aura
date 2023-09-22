// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "HelperFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Get a data table row by tag */
	template<typename T>
	static T* GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag);
};

/** Get a data table row by tag */
template<typename T>
T* UHelperFunctionLibrary::GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
