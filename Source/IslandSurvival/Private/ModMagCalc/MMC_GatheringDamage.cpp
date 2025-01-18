// Fill out your copyright notice in the Description page of Project Settings.


#include "ModMagCalc/MMC_GatheringDamage.h"

#include "Game/ISAttributeSet.h"

UMMC_GatheringDamage::UMMC_GatheringDamage()
{
	PlayerWeaponAttack.AttributeToCapture = UISAttributeSet::GetWeaponAttackAttribute();
	PlayerWeaponAttack.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PlayerWeaponAttack.bSnapshot = false;
	

	RelevantAttributesToCapture.Add(PlayerWeaponAttack);
}

float UMMC_GatheringDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float WeaponAttack = 0.f;
	GetCapturedAttributeMagnitude(PlayerWeaponAttack,Spec,EvaluateParameters,WeaponAttack); //尝试从GE应用的Spec中捕捉到对应的角色属性
	WeaponAttack = FMath::Max<float>(WeaponAttack,0.f);
	
	return 8 + WeaponAttack;
}
