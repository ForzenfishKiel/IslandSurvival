// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController/ISWidgetController.h"
#include "ISMainUIWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangeSignature,float,NewValue);  //绑定玩家属性变化
UCLASS(BlueprintType, Blueprintable)
class ISLANDSURVIVAL_API UISMainUIWidgetController : public UISWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallBackDependencies() override;

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnMaxHealthChange;  //最大生命值变化绑定

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnHealthChange;  //生命值变化绑定

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnVigorChange;//体力值变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnMaxVigorChange;  //最大体力值变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnHungerChange;  //饱食度变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnMaxHungerChange;//最大饱食度变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnThirstChange;  //口渴值变化

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnPlayerStateChangeSignature OnMaxThirstChange;//最大口渴值变化
};
