// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Havesting/ISHarvestingBase.h"
#include "ISLargetItemMaster.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISLargetItemMaster : public AISHarvestingBase
{
	GENERATED_BODY()
public:
	AISLargetItemMaster();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootSceneComponent;
};
