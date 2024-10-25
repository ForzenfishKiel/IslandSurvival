// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "ISItemsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWhenItemPickUp,FItemInformation,TargetItemInfo);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISItemsContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UISItemsContainer();
	bool CheckEmptySlotInInventory(const TArray<FItemInformation>&TargetInventory);
	UFUNCTION(Server,Reliable)
	void ToPickUpItemsInBackPack(const FItemInformation Information);  //拾取物品函数，在服务器上运行
	UPROPERTY(BlueprintReadOnly,Replicated)
	TArray<FItemInformation>InventoryContainer;  //背包
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
