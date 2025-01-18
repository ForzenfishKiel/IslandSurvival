// Fill out your copyright notice in the Description page of Project Settings.


#include "ModMagCalc/MMC_Attack.h"

#include "Game/ISAttributeSet.h"

UMMC_Attack::UMMC_Attack()
{
	PlayerWeaponAttack.AttributeToCapture = UISAttributeSet::GetWeaponAttackAttribute();
	PlayerWeaponAttack.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PlayerWeaponAttack.bSnapshot = false;

	RelevantAttributesToCapture.Add(PlayerWeaponAttack);
}

float UMMC_Attack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float WeaponAttack = 0.f;
	GetCapturedAttributeMagnitude(PlayerWeaponAttack,Spec,EvaluateParameters,WeaponAttack);
	WeaponAttack = FMath::Max(WeaponAttack,0.f);

	//角色攻击力主要来源于武器攻击力
	return WeaponAttack;
}
