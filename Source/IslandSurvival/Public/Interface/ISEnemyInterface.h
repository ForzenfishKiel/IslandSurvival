// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ISAIController.h"
#include "UObject/Interface.h"
#include "ISEnemyInterface.generated.h"

class AISEnemy;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AISAIController* GetAIController();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AISEnemy* GetEnemy();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EAIState GetAIState() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAIState(EAIState State);
};
