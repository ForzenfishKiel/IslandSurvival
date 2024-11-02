// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISCollectibleDataAsset.h"
#include "UObject/Interface.h"
#include "ISCollectibleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISCollectibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISCollectibleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FName GetTargetName() = 0;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	ECollectibleClass GetTargetClass();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void CollectionExecution(AActor*TargetActor,AActor*TargetTool);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ApplyDamageToTarget(AActor*TargetActor);  //应用武器伤害
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetNumsFromMultiplier(AActor*TargetTool,int32 TargetNums);
};
