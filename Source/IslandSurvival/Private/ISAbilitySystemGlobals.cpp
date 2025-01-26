// Fill out your copyright notice in the Description page of Project Settings.


#include "ISAbilitySystemGlobals.h"

#include "ISAbilityTypes.h"

FGameplayEffectContext* UISAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FISGameplayEffectContext();  //生成我们新的自定义的Context结构体
}
