// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ISInventorySystem.h"
#include "Items/ISItemBase.h"
#include "ISPlayerItemDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemInformation : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	int32 ItemID = -1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	int32 ItemQuantity = 0;  //物品数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	UTexture2D* ItemIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	FName ItemName;  //物品名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	FName ItemDesc; //物品介绍
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	int32 ItemDamage = 0;  //这个不是武器造成的伤害，这个是一些工具会造成的固定伤害，例如镐子，斧子等
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	EItemRarity TargetItemRarity = EItemRarity::None;  //武器稀有度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	int32 ItemCurHP = 0;  //物品当前耐久度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	int32 ItemCurMaxHP = 0;  //物品最大耐久度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	bool CanStack = false;  //是否可堆叠
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	int32 MaxStack = 0; //最大可堆叠上限
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	EItemType ItemType = EItemType::None;  //物品种类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorType ArmorType;  //护甲种类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	TSubclassOf<AISItemBase> ItemClassRef;  //物品类型引用
};
UCLASS()
class ISLANDSURVIVAL_API UISPlayerItemDataTable : public UObject
{
	GENERATED_BODY()
public:
	int32 GetItemCount() const;
	FName GetItemName() const;
	FGuid GetGuid() const;

	void SetGuid(const FGuid& InGuid);
	void SetCount(int32 InCount);
	void SetOwnerInventory(UISInventorySystem* InInventory);
	void SetItemName(FName InName);
	UFUNCTION()
	void OnRep_OwnerInventory();
private:
	UPROPERTY(ReplicatedUsing = OnRep_OwnerInventory)
	TObjectPtr<UISInventorySystem> OwnerInventory;
	UPROPERTY(Replicated)
	int32 ItemCount;  //物品数量，当该数量值小于等于0时将该类单例从服务器上删除
	UPROPERTY(Replicated)
	FName ItemName;  //物品ID，根据ID查表
	UPROPERTY(Replicated)
	FGuid Guid;  //物品数据单例唯一指定标识符

	
	//当物品的所拥有的Inventory更新时，需要告诉客户端，该Item是否真的拥有了Inventory，也就是OwnerInventory是否真的存在，为了确定，我们需要在客户端为物品注册OwnerInventory，在内层背包数据添加时

	void PreDestoryFromReplication();  //同样地，在物品数据彻底删除的时候，提前告之注销其拥有的Owner
	
	virtual bool IsSupportedForNetworking() const override {return true;}  //告诉UE该类支持联网
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
