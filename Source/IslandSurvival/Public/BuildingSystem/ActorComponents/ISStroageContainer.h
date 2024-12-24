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

	UFUNCTION(BlueprintCallable)
	bool CheckForgeFuelList(const UDataTable* TargetDT); //检查熔炉的燃料表
	UFUNCTION(BlueprintCallable)
	bool CheckForgeList(const UDataTable* TargetDT);  //检查熔炉中的制作表
	UFUNCTION(BlueprintCallable)
	float FindFuelTime(const UDataTable* TargetDT);  //返回燃料的时间
	UFUNCTION(BlueprintCallable)
	float FindForgeTime(const UDataTable* TargetDT);  //返回冶炼物的时间
	UFUNCTION(BlueprintCallable)
	int32 FindFuel(const UDataTable* TargetDT);
};
