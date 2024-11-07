// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISWidgetController.h"

void UISWidgetController::SetWidgetControllerParams(const FCharacterParams& WidgetControllerParams)
{
	ISPlayerController = WidgetControllerParams.SourcePC;
	ISPlayerState = WidgetControllerParams.SourcePS;
	ISAbilitySystem = WidgetControllerParams.SourceASC;
	ISAttributeSet = WidgetControllerParams.SourceAS;
}

AISPlayerController* UISWidgetController::GetSourcePC()
{
	if(ISPlayerController)
	{
		SourcePlayerController = Cast<AISPlayerController>(ISPlayerController);
	}
	return SourcePlayerController;
}

UISAbilitySystemComponent* UISWidgetController::GetAbilitySystemComponent()
{
	if(ISAbilitySystem)
	{
		SourceAbilitySystemComponent = Cast<UISAbilitySystemComponent>(ISAbilitySystem);
	}
	return SourceAbilitySystemComponent;
}

UISAttributeSet* UISWidgetController::GetSourceAttributeSet()
{
	if(ISAttributeSet)
	{
		SourceAttributeSet = Cast<UISAttributeSet>(ISAttributeSet);
	}
	return SourceAttributeSet;
}

AISPlayerState* UISWidgetController::GetSourcePlayerState()
{
	if(ISPlayerState)
	{
		SourcePlayerState = Cast<AISPlayerState>(ISPlayerState);
	}
	return SourcePlayerState;
}
