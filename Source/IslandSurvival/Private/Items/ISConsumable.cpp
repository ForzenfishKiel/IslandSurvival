// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISConsumable.h"
#include "Character/ISCharacter.h"

void AISConsumable::UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC)
{
	Super::UseItem(TargetCharacter, TargetASC);
	check(Cast<AISCharacter>(TargetCharacter));  //不是角色不能用
	ApplyEffectToTarget(TargetASC,ConsumableEffectClass);
}

void AISConsumable::ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass)
{
	Super::ApplyEffectToTarget(InASC, EffectClass);
	check(EffectClass);
	FGameplayEffectContextHandle EffectContextHandle = InASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);  //添加效果来源为自身
	FGameplayEffectSpecHandle EffectSpecHandle = InASC->MakeOutgoingSpec(EffectClass,1.f,EffectContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());  //应用效果
}
