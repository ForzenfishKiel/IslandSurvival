// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISBuildInterface.generated.h"

// This class does not need to be modified.
class AISBuildingSystemBase;
UINTERFACE(MinimalAPI)
class UISBuildInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISBuildInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AISBuildingSystemBase*GetBuildingSystemBase();  //获取建筑类引用
};
