// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "DamageTextWidgetComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	/** Set damage's text on widget */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bInIsBlockedHit, bool bInIsCriticalHit);
};
