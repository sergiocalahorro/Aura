// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

// Headers - Aura
#include "AttributeSet.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"

#include "AttributeMenuWidgetController.generated.h"

// Forward declarations - Aura
class UAttributesInfo;
struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

#pragma region CORE

public:

	/** Broadcast initial values */
	virtual void BroadcastInitialValues() override;

	/** Bind callbacks to delegates */
	virtual void BindCallbacksToDelegates() override;

private:

	/** Broadcast attribute info */
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
	
public:

	/** Delegate called to broadcast attribute info */
	UPROPERTY(BlueprintAssignable)
	FAttributeInfoSignature AttributeInfoDelegate;

protected:

	/** Data Asset containing attributes' info */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributesInfo> AttributesInfo;

#pragma endregion CORE
};
