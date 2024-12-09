// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISInventorySystem.generated.h"
struct FInventoryConfig;
struct FItemInformation;
/**
 * 
 */
class UISItemsContainer;
class UISPlayerItemDataTable;
UCLASS()
class ISLANDSURVIVAL_API UISInventorySystem : public UObject
{
	GENERATED_BODY()
public:
	UISPlayerItemDataTable* GetItemNameFormGuid(const FGuid& InGuid);
	int32 GetItemCountByName(const FName& InName);
	static UISInventorySystem* CreateInventory(UISItemsContainer* InOwnerComponent,const FInventoryConfig InventoryConfig);  //建立一个方法用于创建自身
	bool AddItems(const TArray<FItemInformation>& InItems);
	bool RemoveItems(const TArray<FItemInformation>& InItemsInfo);
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);
	bool RemoveItemByGuid(const FGuid& InGuid,int32 Count);
	
	void ClientRegisterReplicatedItem(UISPlayerItemDataTable* InItem);
	void ClientUnRegisterReplicatedItem(UISPlayerItemDataTable* InItem);
	
	void MarkItemChange();  //内层背包数据变化通知
private:
	UPROPERTY(Replicated)
	UISItemsContainer* OwnerComponent;  //背包拥有的管理组件
	
	UPROPERTY(Replicated)
	FName Tag;  //背包拥有的名字

	UPROPERTY(Replicated)
	int32 Size;  //背包大小

	UPROPERTY()
	TArray<UISPlayerItemDataTable*> Items;  //物品数据类储存

	UPROPERTY()
	TMap<FGuid, UISPlayerItemDataTable*> ItemMap;  //根据唯一全局指定标识符查找对应的物品数据类

	bool ItemChanged = false;

	UISPlayerItemDataTable* AddItem_Internal();  //添加内层数据，也就是联网数据
	void RemoveItem_Internal(UISPlayerItemDataTable* InItem);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override {return true;}  //告诉UE该类可复制
};