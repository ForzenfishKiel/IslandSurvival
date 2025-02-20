// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
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
// 序列化运算符实现
inline FArchive& operator<<(FArchive& Ar, FItemInformation& ItemInfo) {
	Ar << ItemInfo.ItemID;
	Ar << ItemInfo.ItemQuantity;
	Ar << ItemInfo.ItemIcon;
	Ar << ItemInfo.ItemName;
	Ar << ItemInfo.ItemDesc;
	Ar << ItemInfo.ItemCurHP;
	Ar << ItemInfo.ItemCurMaxHP;
	Ar << ItemInfo.CanStack;
	Ar << ItemInfo.MaxStack;
	Ar << ItemInfo.TargetItemRarity;
	Ar << ItemInfo.ItemType;
	Ar << ItemInfo.ArmorType;
	Ar << ItemInfo.ItemClassRef;
	return Ar;
}
UCLASS()
class ISLANDSURVIVAL_API UISPlayerItemDataTable : public UObject
{
	GENERATED_BODY()
};
