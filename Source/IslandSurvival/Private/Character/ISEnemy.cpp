// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISEnemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/ISMainUIBase.h"

AISEnemy::AISEnemy()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;

	ISEnemyAbilitysystem = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("ISEnemyAbilityComponent"));
	ISEnemyAbilitysystem->SetIsReplicated(true);
	ISEnemyAbilitysystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);  //仅复制最少量信息
	ISEnemyAttribute = CreateDefaultSubobject<UISAttributeSet>(TEXT("ISEnemyAttribute"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void AISEnemy::SetAISpeed(const EAISpeed InState)
{
	GetCharacterMovement()->MaxWalkSpeed = *AISpeedManager.Find(InState);
}

void AISEnemy::Die()
{
	
}

void AISEnemy::MulticastHandleDeath()
{
	Super::MulticastHandleDeath();
	
}

UAbilitySystemComponent* AISEnemy::GetAbilitySystemComponent() const
{
	return ISEnemyAbilitysystem;
}

void AISEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!HasAuthority()) return;  //确保只在服务器运行

	ISEnemyAIController = Cast<AISAIController>(NewController);

	ISEnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

	ISEnemyAIController->RunBehaviorTree(BehaviorTree);  //运行行为树
	
}

void AISEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();  //初始化角色技能信息

	if(HasAuthority())
	{
		UISAbilitysystemLibary::GiveStartupAbilities(this,ISEnemyAbilitysystem,CharacterName);
	}

	if(UISMainUIBase* HealthWidget = Cast<UISMainUIBase>(HealthBar->GetUserWidgetObject()))
	{
		HealthWidget->SetWidgetController(this);
	}

	if(const UISAttributeSet* AS = Cast<UISAttributeSet>(GetEnemyAttribute()))
	{
		ISEnemyAbilitysystem->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda([this]
				(const FOnAttributeChangeData& Data)
			{
				OnAttributeChange.Broadcast(Data.NewValue);
			}
		);
		ISEnemyAbilitysystem->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda([this]
		(const FOnAttributeChangeData& Data)
			{
				OnAttributeChange.Broadcast(Data.NewValue);
			}
		);
		OnAttributeChange.Broadcast(AS->GetHealth());
		OnAttributeChange.Broadcast(AS->GetMaxHealth());
	}
}

UAttributeSet* AISEnemy::GetEnemyAttribute() const
{
	return ISEnemyAttribute;
}

void AISEnemy::InitAbilityActorInfo()
{
	ISEnemyAbilitysystem->InitAbilityActorInfo(this, this);

	if(HasAuthority())
	{
		InitializePlayerAttribute(ISEnemyAbilitysystem,nullptr);
	}
}
//初始化角色属性
void AISEnemy::InitializePlayerAttribute(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> AttributeClass)
{
	UISAbilitysystemLibary::InitializeCharacterAttributes(this,CharacterName,Level,ASC);
}

void AISEnemy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AISEnemy,AIState);
}

int32 AISEnemy::GetLevel_Implementation()
{
	return Level;
}

AISAIController* AISEnemy::GetAIController_Implementation()
{
	return Cast<AISAIController>(GetController());
}

EAIState AISEnemy::GetAIState_Implementation() const
{
	return AIState;
}

void AISEnemy::SetAIState_Implementation(EAIState State)
{
	AIState = State;
}
