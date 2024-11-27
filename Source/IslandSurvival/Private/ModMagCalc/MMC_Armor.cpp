// Fill out your copyright notice in the Description page of Project Settings.


#include "ModMagCalc/MMC_Armor.h"

#include "Game/ISAttributeSet.h"

UMMC_Armor::UMMC_Armor()
{
	GearArmorDef.AttributeToCapture = UISAttributeSet::GetGearArmorAttribute(); //设置需要获取的属性对象
	GearArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; //设置拾取对象为GE的应用目标
	GearArmorDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(GearArmorDef);
}

float UMMC_Armor::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//  从 source 和 target 获取 Tag
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float GearArmor = 0.f;
	GetCapturedAttributeMagnitude(GearArmorDef, Spec, EvaluateParameters, GearArmor);
	GearArmor = FMath::Max<float>(GearArmor, 0.f);
	
	return 20+GearArmor;
}
