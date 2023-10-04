// Copyright Sergio Calahorro

#include "GAS/Attributes/Data/AttributesInfo.h"

// Headers - Aura
#include "GAS/Attributes/Data/AuraAttributeInfo.h"

/** Find Attribute's info by GameplayTag */
FAuraAttributeInfo UAttributesInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributesInformation)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("UAttributeInfo::FindAttributeInfoForTag - Can't find Info for AttributeTag [%s] on AttributesInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}