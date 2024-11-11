// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISMainUIWidgetController.h"

void UISMainUIWidgetController::BindCallBackDependencies()
{
	Super::BindCallBackDependencies();

	
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
		FString HealthText = FString::Printf(TEXT("Health Changed: %f"), Data.NewValue);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, HealthText);  // -1表示消息唯一标识，5.0f表示显示时长
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
		FString HealthText = FString::Printf(TEXT("Health Changed: %f"), Data.NewValue);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, HealthText);  // -1表示消息唯一标识，5.0f表示显示时长
		OnThirstChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxThirstAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxThirstChange.Broadcast(Data.NewValue);
	});
}
