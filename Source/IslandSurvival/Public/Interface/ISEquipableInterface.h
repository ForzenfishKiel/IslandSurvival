// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISEquipableInterface.generated.h"

class AISEquipable;
enum class EItemRarity : uint8;
struct FEquipInfoConfig;
enum class ECharacterEquipState : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISEquipableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISEquipableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ECharacterEquipState GetTargetEquipState() = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndAbilityActivate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EItemRarity GetItemRarity();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AISEquipable* GetEquipable() ;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnEquipableWasInteract(AActor* InteractingActor);
};
