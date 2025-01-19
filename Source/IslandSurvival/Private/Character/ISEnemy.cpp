// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISEnemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"

AISEnemy::AISEnemy()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;

	ISEnemyAbilitysystem = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("ISEnemyAbilityComponent"));
	ISEnemyAbilitysystem->SetIsReplicated(true);
	ISEnemyAbilitysystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);  //仅复制最少量信息
	ISEnemyAttribute = CreateDefaultSubobject<UISAttributeSet>(TEXT("ISEnemyAttribute"));
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
	
}

int32 AISEnemy::GetLevel_Implementation()
{
	return Level;
}
