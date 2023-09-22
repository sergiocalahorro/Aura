// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"

#include "UIWidgetRow.generated.h"

// Forward declarations - Unreal Engine
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Tag;

	/** Message */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message;

	/** Widget class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> WidgetClass;

	/** Image */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
};
