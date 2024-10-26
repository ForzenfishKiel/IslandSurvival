// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISItemInterface.generated.h"

enum class EItemType : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PickUpItemToInventory(APawn*TargetPawn,AActor* TargetActor);
};
