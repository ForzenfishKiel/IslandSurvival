// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISNPCInterface.generated.h"

class AISNonePlayerCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISNPCInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISNPCInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AISNonePlayerCharacter* GetNPC();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnNPCWasInteracted(AActor* InteractingActor);  //当挂在接口的对象被交互时
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetFavorability() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetCharacterName() const;
};
