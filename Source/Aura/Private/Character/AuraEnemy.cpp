// Copyright Sergio Calahorro

#include "Character/AuraEnemy.h"

// Headers - Unreal Engine
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Headers - Aura
#include "Aura.h"
#include "AI/AuraAIController.h"
#include "GameplayTags/AuraGameplayTags.h"
#include "GAS/AbilitySystem/AuraAbilitySystemComponent.h"
#include "GAS/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GAS/Attributes/AuraAttributeSet.h"
#include "UI/Widget/AuraUserWidget.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// Movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Health bar
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);

	// GAS setup
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

#pragma endregion INITIALIZATION

#pragma endregion OVERRIDES

/** Called when this Pawn is possessed. Only called on the server (or in standalone) */
void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
	{
		return;
	}
	
	AuraAIController = CastChecked<AAuraAIController>(NewController);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	SetInitialBlackboardValues();
}

/** Called when the game starts or when spawned */
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	AddCharacterAbilities();
	SetupEnemy();
}

#pragma endregion OVERRIDES

#pragma region AI

/** Set initial values for Blackboard */
void AAuraEnemy::SetInitialBlackboardValues() const
{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

#pragma endregion AI

#pragma region COMBAT

/** Get level */
int32 AAuraEnemy::GetCurrentLevel() const
{
	return Level;
}

/** Set new combat target */
void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

/** Get current combat target */
AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

/** Functionality performed on death */
void AAuraEnemy::Death()
{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	SetLifeSpan(LifeSpan);
	Super::Death();
}

/** Callback called whenever HitReact's tag is changed */
void AAuraEnemy::HitReactTagChanged(const FGameplayTag GameplayTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController)
	{
		if (UBlackboardComponent* BlackboardComponent = AuraAIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsBool(FName("HitReacting"), bHitReacting);
		}
	}
}

#pragma endregion COMBAT

#pragma region INTERACTABLE

/** Functionality performed when Actor should be highlighted (on hovered) */
void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

/** Functionality performed when Actor should be un-highlighted (stopped being hovered) */
void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

#pragma endregion INTERACTABLE

#pragma region GAS

/** Initialize ability actor info */
void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
		UAuraAbilitySystemLibrary::GiveDefaultAbilities(this, AbilitySystemComponent, CharacterClass);
	}
}

/** Setup health logic and listening for changes on tags */
void AAuraEnemy::SetupEnemy()
{
	// Listen for changes on HitReact tag
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact).AddUObject(this, &AAuraEnemy::HitReactTagChanged);

	// Set health bar widget's controller
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	// Set callbacks for changes on Health and MaxHealth Attributes
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Broadcast initial Health and MaxHealth Attributes' values
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	// Set default values
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

#pragma endregion GAS
