// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/ISAIEnum.h"
#include "UI/ISMenuUIBase.h"
#include "UObject/Interface.h"
#include "ISPublicInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISPublicInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISPublicInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UISMenuUIBase* GetMenuUI() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindWidgetController(AActor* TargetActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EAIState GetAIState() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAIState(EAIState State);
};
