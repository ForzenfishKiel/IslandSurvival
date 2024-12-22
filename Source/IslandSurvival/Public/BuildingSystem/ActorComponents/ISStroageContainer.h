// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISCraftingComponent.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISStroageContainer.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISStroageContainer : public UISItemsContainer
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ISStroageContainer")
	void CraftAction(UISCraftingComponent* TargetCraftingComponent,const UDataTable* TargetDT,const int32 TargetID);
	UFUNCTION(BlueprintCallable)
	bool CheckStorageEmptySlots();  //检查是否还有空的物品
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void SaveToStorageContainer(const int32 TargetID);  //制作好的物品将存放在工作台库存
protected:
	virtual void BeginPlay() override;
};
