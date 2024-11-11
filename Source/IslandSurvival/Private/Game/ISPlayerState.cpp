// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerState.h"

#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"

AISPlayerState::AISPlayerState()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;
	ISAbilitySystemComponent = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("ISAbilitySystemComponent"));
	ISAbilitySystemComponent->SetIsReplicated(true);  //设置复制行为为可复制
	ISAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);  //设置ASC仅仅被复制到拥有者的客户端
	ISAttributeSet = CreateDefaultSubobject<UISAttributeSet>(TEXT("ISAttributeSet"));

}

UAbilitySystemComponent* AISPlayerState::GetAbilitySystemComponent() const
{
	return ISAbilitySystemComponent;
}

void AISPlayerState::AddToLevel(int32 InLevel)  //等级累加
{
	CurrentLevel+=InLevel;
	OnPlayerLevelChanged.Broadcast(CurrentLevel);
}

void AISPlayerState::SetLevel(int32 InLevel)  //等级设置
{
	CurrentLevel = InLevel;
	OnPlayerLevelChanged.Broadcast(CurrentLevel);
}

void AISPlayerState::AddToXP(int32 InXP)
{
	CurrentXP+=InXP;
	OnPlayerXPChange.Broadcast(CurrentXP);
}

void AISPlayerState::SetXP(int32 InXP)
{
	CurrentXP = InXP;
	OnPlayerXPChange.Broadcast(CurrentXP);
}

void AISPlayerState::AddToAttributePoint(int32 InAttributePoint)
{
	AttributePoint+=InAttributePoint;
	OnPlayerAttributePointChange.Broadcast(AttributePoint);
}

void AISPlayerState::SetAttributePoint(int32 InAttributePoint)
{
	
}
