// Fill out your copyright notice in the Description page of Project Settings.


#include "ModMagCalc/MMC_MaxHealth.h"

#include "Interface/ISPlayerInterface.h"

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//  从 source 和 target 获取 Tag
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	int32 Level = 0;
	if(Spec.GetContext().GetSourceObject()->Implements<UISPlayerInterface>())
	{
		Level = IISPlayerInterface::Execute_GetLevel(Spec.GetContext().GetSourceObject());  //获取角色等级
	}

	int32 MaxHealth = 0;
	if(Spec.GetContext().GetSourceObject()->Implements<UISPlayerInterface>())
	{
		MaxHealth = IISPlayerInterface::Execute_GetPlayerMaxHealthPoint(Spec.GetContext().GetSourceObject());  //获取角色最大生命值加点
	}
	return Level * 2.5+MaxHealth * 5;  //返回最后输出的值
}
