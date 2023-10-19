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

	/** Shuffle array */
	template<typename T>
	static void ShuffleArray(TArray<T>& Array);
};

/** Get a data table row by tag */
template<typename T>
T* UHelperFunctionLibrary::GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}

/** Shuffle array */
template<typename T>
void UHelperFunctionLibrary::ShuffleArray(TArray<T>& Array)
{
	const int32 LastIndex = Array.Num() - 1;
	for (int32 i = 0; i <= LastIndex; i += 1)
	{
		const int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			Array.Swap(i, Index);
		}
	}
}