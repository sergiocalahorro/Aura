// Copyright Sergio Calahorro

#include "Character/Data/CharacterClassInfo.h"

/** Get the default information for the given class */
FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}