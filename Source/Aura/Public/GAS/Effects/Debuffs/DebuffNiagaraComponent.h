// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"

#include "DebuffNiagaraComponent.generated.h"

UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Sets default values for this object's properties */
	UDebuffNiagaraComponent();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region DEBUFF

protected:

	/** Callback called whenever there's a change on the debuff tag */
	void OnDebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	/** Callback called when the owner is dead */
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
	
public:

	/** Debuff's tag */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DebuffTag;

#pragma endregion DEBUFF
};
