// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISCharacterInventory.generated.h"
/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISCharacterInventory : public UISItemsContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 InventorySpace = 30;  //背包空间
	FItemInformation ItemInfo;
	UPROPERTY(BlueprintAssignable)
	FWhenItemPickUp ItemPickup;
	void InitializeBackPackSpace();
protected:
	virtual void BeginPlay() override;
};
