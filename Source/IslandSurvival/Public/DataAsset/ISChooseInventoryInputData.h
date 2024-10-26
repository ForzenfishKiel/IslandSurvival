// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ISChooseInventoryInputData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FChooseInventory
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	int32 ChooseIndex = -1;
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction*InputAction;
};
UCLASS()
class ISLANDSURVIVAL_API UISChooseInventoryInputData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FChooseInventory> ChooseInventory;
};
