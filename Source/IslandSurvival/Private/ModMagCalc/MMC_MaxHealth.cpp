// Fill out your copyright notice in the Description page of Project Settings.


#include "ModMagCalc/MMC_MaxHealth.h"

#include "Game/ISAttributeSet.h"
#include "Interface/ISPlayerInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	PlayerLevelDef.AttributeToCapture = UISAttributeSet::GetPlayerLevelAttribute(); //设置需要获取的属性对象
	PlayerLevelDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; //设置拾取对象为GE的应用目标
	PlayerLevelDef.bSnapshot = false;

	PlayerMaxHealthLevelDef.AttributeToCapture = UISAttributeSet::GetMaxHealthLevelAttribute();
	PlayerMaxHealthLevelDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PlayerMaxHealthLevelDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(PlayerLevelDef); //添加到捕获属性数值，只有添加到列表，才会去获取属性值
	RelevantAttributesToCapture.Add(PlayerMaxHealthLevelDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//  从 source 和 target 获取 Tag
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	//获取等级
	float Level = 0.f;
	GetCapturedAttributeMagnitude(PlayerLevelDef, Spec, EvaluateParameters, Level);
	Level = FMath::Max<float>(Level, 0.f);

	//获取加点等级
	float MaxHealthLevel = 0.f;
	GetCapturedAttributeMagnitude(PlayerMaxHealthLevelDef,Spec,EvaluateParameters,MaxHealthLevel);
	MaxHealthLevel = FMath::Max<float>(MaxHealthLevel, 0.f);

	float AttributeEval = 0.f;

	//从角色的状态里获取加点的对应乘区值
	if(Spec.GetContext().GetSourceObject()->Implements<UISPlayerInterface>())
	{
		AISPlayerState*SourcePlayerState = IISPlayerInterface::Execute_GetPlayerState(Spec.GetContext().GetSourceObject());
		check(SourcePlayerState);
		UCurveTable*AttributePointCT = SourcePlayerState->PlayerAttributePointsTable;  //获取对应CT表
		if(!AttributePointCT) return 0.f;
		const FRealCurve*MaxHealthAttributePoint = AttributePointCT->FindCurve(FName("MaxHealthAttributePoint"),FString());
		if(!MaxHealthAttributePoint) return 0.f;
		const int32 EvalValue = MaxHealthLevel;
		AttributeEval = MaxHealthAttributePoint->Eval(EvalValue);  //获取角色最大生命值加点);  // 从已经增长的最大生命值技能点等级中获取其加点值
	}
	
	return 100 + (100 * Level*0.2)+100 * AttributeEval;  //返回最后输出的值
}
