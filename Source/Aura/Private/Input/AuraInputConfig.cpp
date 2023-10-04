// Copyright Sergio Calahorro

#include "Input/AuraInputConfig.h"

/** Find an ability's input action by its GameplayTag */
const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const UInputAction* InputAction = *AbilityInputActions.Find(InputTag))
	{
		return InputAction;
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("UAuraInputConfig::FindAbilityInputActionForTag - Can't find InputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}