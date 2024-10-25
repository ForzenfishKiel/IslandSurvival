// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ISGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
	TObjectPtr<UDataTable>ItemDataTable;
};
