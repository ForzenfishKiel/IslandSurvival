// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "BuildingSystem/ActorComponents/ISStroageContainer.h"
#include "ISStorageMaster.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISStorageMaster : public AISBuildingSystemBase
{
	GENERATED_BODY()
public:
	AISStorageMaster();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISStroageContainer>StorageContainer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget>InteractUserWidget;
};
