// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ISItemBase.h"
#include "UObject/NoExportTypes.h"
#include "ISCraftItemInformation.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECraftingWorkSpaceType : uint8
{
	None UMETA(DisplayName = "None"),
	PlayerInventory = 1 UMETA(DisplayName = "PlayerInventory"),//玩家库存

};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None UMETA(DisplayName = "None"),
	Misc UMETA(DisplayName = "Misc"),  //杂项
	Tools UMETA(DisplayName = "Tools"),
	Armor UMETA(DisplayName = "Armor"),
	Structure UMETA(DisplayName = "Structure"), //建筑
};
USTRUCT(BlueprintType)
struct FItemRequire
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemID;  //需要的物品ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemQuantity;  //需要的数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* TargetIcon;  //需要的对方的图片
};
USTRUCT(BlueprintType)
struct FItemRecipe : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemDescription;  //物品介绍
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredLevel;  //需要的物品等级
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemRequire> ItemRequired;  //需要的物品陈列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemRequire ItemRequireSingle;  //需要的物品
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemCategory ItemCategory;  //物品类别
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ItemExperience;
};
UCLASS()
class ISLANDSURVIVAL_API UISCraftItemInformation : public UObject
{
	GENERATED_BODY()
	
};
