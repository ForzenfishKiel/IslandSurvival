// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISEnemy.h"

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

UAttributeSet* AISEnemy::GetEnemyAttribute() const
{
	return ISEnemyAttribute;
}
