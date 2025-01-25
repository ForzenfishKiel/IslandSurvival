// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISNonePlayerCharacter.h"

#include "Game/ISAbilitySystemComponent.h"

AISNonePlayerCharacter::AISNonePlayerCharacter()
{
	NetUpdateFrequency = 100.f;  //网络更新速率
	MinNetUpdateFrequency = 66.f;
	
	NPCAsc = CreateDefaultSubobject<UISAbilitySystemComponent>(TEXT("NPCAbilitySystemComponent"));
	NPCAsc->SetIsReplicated(true);
	NPCAsc->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); //最小地去复制npc的一些属性
	NPCTradComponent = CreateDefaultSubobject<UISTradingSystemComponent>(TEXT("TradingSystemComponent"));  //创建可交易允许的组件
}

AISNonePlayerCharacter* AISNonePlayerCharacter::GetNPC_Implementation()
{
	return this;
}

void AISNonePlayerCharacter::OnNPCWasInteracted_Implementation(AActor* InteractingActor)
{
	IISNPCInterface::OnNPCWasInteracted_Implementation(InteractingActor);
}

//调用NPC的获取其ASC的方法
UAbilitySystemComponent* AISNonePlayerCharacter::GetAbilitySystemComponent() const
{
	return NPCAsc;
}
