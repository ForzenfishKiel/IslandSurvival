// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController/ISWidgetController.h"
#include "ISMenuWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPProgressPercentChange, float, XPPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMenuStateChangeSignature,float,NewValue);  //绑定玩家属性变化
UCLASS(BlueprintType, Blueprintable)
class ISLANDSURVIVAL_API UISMenuWidgetController : public UISWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallBackDependencies() override;
	virtual void BroadcastInitialValues() override;
	void OnXPChange(int32 CurrentXP);

	UPROPERTY(BlueprintAssignable)
	FOnXPProgressPercentChange OnXPProgressPercent;

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnMaxHealthChange;  //最大生命值变化绑定

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnHealthChange;  //生命值变化绑定

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnVigorChange;//体力值变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnMaxVigorChange;  //最大体力值变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnHungerChange;  //饱食度变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnMaxHungerChange;//最大饱食度变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnThirstChange;  //口渴值变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerMenuStateChangeSignature OnMaxThirstChange;//最大口渴值变化
};
