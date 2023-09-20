// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region CORE

public:

	/** Get WidgetController */
	UObject* GetWidgetController() const;

	/** Set WidgetController */
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:

	/** Functionality performed once WidgetController is set */
	UFUNCTION(BlueprintNativeEvent)
	void OnWidgetControllerSet();
	void OnWidgetControllerSet_Implementation();
	
private:

	/** Widget's Controller reference */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Core", meta = (AllowPrivateAccess = true))
	TObjectPtr<UObject> WidgetController;

#pragma endregion CORE
	
};
