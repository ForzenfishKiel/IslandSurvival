// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Character/ISCharacterBase.h"
#include "Game/ISAbilitySystemComponent.h"
#include "Game/ISAttributeSet.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerState.h"
#include "UObject/NoExportTypes.h"
#include "ISWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ISLANDSURVIVAL_API UISWidgetController : public UObject
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues(){return;}
	virtual void BindCallBackDependencies(){return;}
	virtual void SetWidgetControllerParams(const FCharacterParams&WidgetControllerParams);
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> ISAbilitySystem = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> ISPlayerController = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> ISPlayerState = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> ISAttributeSet = nullptr;

	/**********************************************************************************************************/
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AISPlayerController> SourcePlayerController;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AISPlayerState> SourcePlayerState;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UISAbilitySystemComponent> SourceAbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UISAttributeSet> SourceAttributeSet;

	AISPlayerController*GetSourcePC();
	UISAbilitySystemComponent* GetAbilitySystemComponent();
	UISAttributeSet* GetSourceAttributeSet();
	AISPlayerState* GetSourcePlayerState();
};
