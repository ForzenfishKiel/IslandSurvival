// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc/ExecCalc_InRecoverCoins.h"
#include "AbilitySystemComponent.h"
#include "ISAbilityTypes.h"
#include "BlueprintFunctionLibary/ISAbilitysystemLibary.h"
#include "DataAsset/ISTraderSpecialData.h"
#include "Game/ISAttributeSet.h"
#include "Interface/ISNPCInterface.h"


void UExecCalc_InRecoverCoins::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                      FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;  //获取自身
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;  //获取对方

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();  //从效果实例中获取上下文句柄
	FISGameplayEffectContext* SourceEffectContext = static_cast<FISGameplayEffectContext*>(EffectContextHandle.Get());

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	/*开始计算*/

	//导入交易的价格表
	const UISTraderSpecialData* TraderSpecialData = UISAbilitysystemLibary::GetTraderSpecialData(SourceAvatar);
	if(!TraderSpecialData) return;

	const FTraderSalesData TraderSalesData = TraderSpecialData->GetTraderSalesData(IISNPCInterface::Execute_GetCharacterName(SourceAvatar));
	const UCurveTable* SalesCT = TraderSalesData.CanTradeCurveTable;
	if(!SalesCT) return;  //如果价格表为空直接退回

	const FRealCurve* SalseRealCT = SalesCT->FindCurve(SourceEffectContext->GetTargetSaleID(),FString());  //获取对应的价格CT
	if(!SalseRealCT) return;
	
	float ResultValue = SalseRealCT->Eval(IISNPCInterface::Execute_GetFavorability(SourceAvatar));  //从当前好感度中获取对应的价格

	ResultValue *= 0.5;  //出售的价格乘以50%

	const FGameplayModifierEvaluatedData EvaluatedData(UISAttributeSet::GetInRecoverCoinsAttribute(),EGameplayModOp::Override,ResultValue);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);  
}
