// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "Data/ISPlayerItemDataTable.h"
#include "UObject/NoExportTypes.h"
#include "ISInventorySystem.generated.h"

/**
 * 用于服务器处理的库存网络层
 */
UCLASS()
class ISLANDSURVIVAL_API UISInventorySystem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_InventoryChange)
	TArray<FItemInformation> InternetInventory;   //用于在网络显示的背包

	FItemInformation ItemInfo;

	UPROPERTY(Replicated)
	TObjectPtr<UISItemsContainer> OwnerComponent;  //该背包类拥有的管理器

	UPROPERTY(Replicated)
	int32 Size = 0;

	UPROPERTY(Replicated)
	FName InventoryName;

	static UISInventorySystem* CreateInventory(UISItemsContainer* InOwnerContainer);  //开辟一个新的背包空间对象
	UFUNCTION()
	void OnRep_InventoryChange();

	void InitializeInterNetInventory(const int32 InSize);

	void UpdateInventoryDataFromClient(const TArray<FItemInformation>& InItems);

private:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override {return true;}  //告诉UE该类可复制
};
