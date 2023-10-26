// Copyright Sergio Calahorro

#include "GAS/AbilitySystem/Data/AbilitiesInfo.h"

// Headers - Aura
#include "AuraLogChannels.h"

/** Find Ability's info by GameplayTag */
FAuraAbilityInfo UAbilitiesInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiesInformation)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("UAbilitiesInfo::FindAbilityInfoForTag - Can't find Info for AbilityTag [%s] on AbilitiesInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}