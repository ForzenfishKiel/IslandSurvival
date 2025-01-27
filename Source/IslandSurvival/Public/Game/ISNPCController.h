// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ISNPCController.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISNPCController : public AAIController
{
	GENERATED_BODY()
private:
	virtual void OnPossess(APawn* InPawn) override;
};
