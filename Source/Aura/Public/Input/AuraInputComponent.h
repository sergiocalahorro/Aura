// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

// Headers - Aura
#include "AuraInputConfig.h"

#include "AuraInputComponent.generated.h"

UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	/** Bind abilities to input actions */
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

/** Bind abilities to input actions */
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (auto& Pair : InputConfig->AbilityInputActions)
	{
		const FGameplayTag InputTag = Pair.Key;
		const UInputAction* InputAction = Pair.Value;
		
		if (InputAction && InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
			}
		}
	}
}
