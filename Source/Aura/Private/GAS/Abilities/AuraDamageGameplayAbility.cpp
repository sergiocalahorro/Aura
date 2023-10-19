// Copyright Sergio Calahorro

#include "GAS/Abilities/AuraDamageGameplayAbility.h"

// Headers - Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#pragma region DAMAGE

/** Apply damage to target Actor */
void UAuraDamageGameplayAbility::ApplyDamage(AActor* TargetActor)
{
	const int32 Level = GetAbilityLevel();
	const FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, Level);

	for (auto DamageType : DamageTypes)
	{
		const float ScaledDamage = DamageType.Value.GetValueAtLevel(Level);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType.Key, ScaledDamage);
	}

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
	}
}

#pragma endregion DAMAGE