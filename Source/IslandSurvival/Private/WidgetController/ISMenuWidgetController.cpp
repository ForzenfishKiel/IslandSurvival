// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISMenuWidgetController.h"
#include "DataAsset/ISLevelUpInformation.h"

void UISMenuWidgetController::BindCallBackDependencies()
{
	Super::BindCallBackDependencies();
	GetSourcePlayerState()->OnPlayerXPChange.AddUObject(this,&ThisClass::OnXPChange);
	GetSourcePlayerState()->OnPlayerLevelChanged.AddLambda([this](int32 NewLevel)
	{
		OnLevelStateChange.Broadcast(NewLevel);
	});
	GetSourcePlayerState()->OnPlayerAttributePointChange.AddLambda([this](int32 NewPoint)
	{
		OnAttributeStateChange.Broadcast(NewPoint);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetHealthAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnHealthChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxHealthAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetVigorAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnVigorChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxVigorAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxVigorChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetHungerAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnHungerChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxHungerAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxHungerChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetThirstAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnThirstChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxThirstAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxThirstChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetCoinsAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		int NewValue = Data.NewValue;
		FString NewValueString = FString::Printf(TEXT("金币不足！！！ NewValue: %d"), NewValue);

		// 显示调试信息
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, NewValueString);
		OnCoinsChange.Broadcast(Data.NewValue);
	});
}

void UISMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	OnMaxHealthChange.Broadcast(GetSourceAttributeSet()->GetMaxHealth());
	OnHealthChange.Broadcast(GetSourceAttributeSet()->GetHealth());
	OnMaxVigorChange.Broadcast(GetSourceAttributeSet()->GetMaxVigor());
	OnVigorChange.Broadcast(GetSourceAttributeSet()->GetVigor());
	OnMaxHungerChange.Broadcast(GetSourceAttributeSet()->GetMaxHunger());
	OnHungerChange.Broadcast(GetSourceAttributeSet()->GetHunger());
	OnMaxThirstChange.Broadcast(GetSourceAttributeSet()->GetMaxThirst());
	OnThirstChange.Broadcast(GetSourceAttributeSet()->GetThirst());
	OnCoinsChange.Broadcast(GetSourceAttributeSet()->GetCoins());
}

//经验值更改广播
void UISMenuWidgetController::OnXPChange(int32 CurrentXP)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("111")));
	UISLevelUpInformation*IsLevelUpInformation = GetSourcePlayerState()->ISLevelUpInformation;  //获取角色的等级信息表
	check(IsLevelUpInformation);
	const int32 Level = IsLevelUpInformation->FindLevelFromXP(CurrentXP);  //从当前经验值获取当前等级
	const int32 MaxLevel = IsLevelUpInformation->LevelUpArray.Num() - 1;  //角色的最大等级就是数组的最大一位减一(数组0号不计入值)
	//如果角色当前等级不是满级
	//等级进度条的实际值 = 当前值/最大值
	if(Level<=MaxLevel&&Level>=0)
	{
		const int32 RequireXP = IsLevelUpInformation->LevelUpArray[Level].LevelUpRequirement;
		const int32 PreViousLevelUpRequirement = IsLevelUpInformation->LevelUpArray[Level-1].LevelUpRequirement;

		const float XPPercent = static_cast<float>(CurrentXP-PreViousLevelUpRequirement)/(RequireXP-PreViousLevelUpRequirement);
		OnXPProgressPercent.Broadcast(XPPercent);
	}
}
