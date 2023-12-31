﻿// Copyright Sergio Calahorro

#include "Character/AuraCharacter.h"

// Headers - Unreal Engine
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraComponent.h"

// Headers - Aura
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/Experience/Data/LevelUpInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 750.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraComponent"));
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when this Pawn is possessed. Only called on the server (or in standalone) */
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

/** PlayerState Replication Notification Callback. Only called on clients */
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

/** Called when the game starts or when spawned */
void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion OVERRIDES

#pragma region COMBAT

/** Get level */
int32 AAuraCharacter::GetCurrentLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	return AuraPlayerState->GetPlayerLevel();
}

/** RepNotify callback for bIsStunned */
void AAuraCharacter::OnRep_IsStunned()
{
	Super::OnRep_IsStunned();

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
		
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_InputReleased);

		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
		}
	}
}

#pragma endregion COMBAT

#pragma region PLAYER

/** Add incoming XP to accumulated XP */
void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	AuraPlayerState->AddToXP(InXP);
}

/** Get current accumulated XP */
int32 AAuraCharacter::GetXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	return AuraPlayerState->GetXP();
}

/** Get attribute points reward for new level */
int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);

	return LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

/** Get spell points reward for new level */
int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);

	return LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

/** Add incoming level to player's current level */
void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	AuraPlayerState->AddToLevel(InPlayerLevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->UpdateAbilitiesStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

/** Add incoming attribute points to player's current attribute points */
void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

/** Add incoming spell points to player's current spell points */
void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

/** Get player's attribute points */
int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	return AuraPlayerState->GetAttributePoints();
}

/** Get player's spell points */
int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	return AuraPlayerState->GetSpellPoints();
}

/** Handle level up */
void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

/** Find level for incoming XP */
int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	check(LevelUpInfo);
	
	return LevelUpInfo->FindLevelForXP(InXP);
}

/** Multicast RPC for displaying level up particles */
void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	const FVector CameraLocation = Camera->GetComponentLocation();
	const FVector NiagaraComponentLocation = LevelUpNiagaraComponent->GetComponentLocation();
	const FRotator NiagaraComponentToCameraRotation = (CameraLocation - NiagaraComponentLocation).Rotation();
	LevelUpNiagaraComponent->SetWorldRotation(NiagaraComponentToCameraRotation);
	LevelUpNiagaraComponent->Activate(true);
}

/** Show magic circle */
void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
	}
}

/** Hide magic circle */
void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
	}
}

#pragma endregion PLAYER

#pragma region GAS

/** Initialize ability actor info */
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	ASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun).AddUObject(this, &AAuraCharacter::StunTagChanged);
	
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	ApplyEffectToSelf(PrimaryAttributes, 1.f);
	ApplyEffectToSelf(SecondaryAttributes, 1.f);
	ApplyEffectToSelf(VitalAttributes, 1.f);
	ApplyEffectDefinitionsToSelf(DefaultEffects);
}

#pragma endregion GAS
