// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISCombatInterface.generated.h"

class AISCharacterBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	void ActivateCharacterAbility();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	void EndActivateCharacterAbility();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	AISCharacterBase* GetAvaActor();
};
