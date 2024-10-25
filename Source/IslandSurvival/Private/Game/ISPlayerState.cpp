// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerState.h"

#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"

AISPlayerState::AISPlayerState()
{
	ISAbilitySystemComponent = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("ISAbilitySystemComponent"));
	ISAbilitySystemComponent->SetIsReplicated(true);  //设置复制行为为可复制
	ISAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);  //设置ASC仅仅被复制到拥有者的客户端
	ISAttributeSet = CreateDefaultSubobject<UISAttributeSet>(TEXT("ISAttributeSet"));
	NetUpdateFrequency = 100.f;  //网络更新速率
}

UAbilitySystemComponent* AISPlayerState::GetAbilitySystemComponent() const
{
	return ISAbilitySystemComponent;
}
