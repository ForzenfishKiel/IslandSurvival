// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc/ExecCalc_Damage.h"

#include "GameplayTagsManager.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISGameplayTagsManager.h"
#include "Interface/ISCombatInterface.h"

struct ISDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);

	ISDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UISAttributeSet,Attack,Source,false);  //捕捉本地攻击者的Attack
		DEFINE_ATTRIBUTE_CAPTUREDEF(UISAttributeSet,Armor,Target,false); //捕捉对方的护甲
	}
};

static const ISDamageStatic& DamageStatic()
{
	static ISDamageStatic DamageStatic;
	return DamageStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(ISDamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(ISDamageStatic().AttackDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	//获取对方和自身的ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//获取AvatarActor
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	//需要自身和对方都持有战斗接口
	IISCombatInterface* SourceInterface = Cast<IISCombatInterface>(SourceAvatar);
	IISCombatInterface* TargetInterface = Cast<IISCombatInterface>(TargetAvatar);

	if(!SourceInterface && !TargetInterface) return;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();  //从效果实例中获取上下文句柄

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	//正式计算，创建总伤害
	float Damage = Spec.GetSetByCallerMagnitude(FGameplayTagsManager::Get().Attribute_Meta_InComingDamage);  //从当前技能效果的SetByCaller对应的Tag中获取数值

	float TargetArmor = 0.f; //获取对方的护甲值
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmorDef,EvaluateParams,TargetArmor);
	TargetArmor = FMath::Max(0.f,TargetArmor);

	float SourceAttack = 0.f ;//获取自身的总攻击力
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AttackDef,EvaluateParams,SourceAttack);
	SourceAttack = FMath::Max(0.f,SourceAttack);

	Damage = SourceAttack - TargetArmor;

	//设定该Effect的计算的值会作用于IncomingDmaage并且为覆盖
	const FGameplayModifierEvaluatedData EvaluatedData(UISAttributeSet::GetInComingDamageAttribute(),EGameplayModOp::Override,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
