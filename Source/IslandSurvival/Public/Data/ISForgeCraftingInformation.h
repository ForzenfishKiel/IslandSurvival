// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISCraftItemInformation.h"
#include "UObject/NoExportTypes.h"
#include "ISForgeCraftingInformation.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FForgeRequire
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
struct FForgeRecipe : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName = FText();  //物品名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemID;  //物品ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemDescription;  //物品介绍
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon;  //物品UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FForgeRequire> ItemRequired;  //需要的物品陈列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ItemExperience;  //给予经验
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FiringTime = 0.f;  //烧制时间
};
UCLASS()
class ISLANDSURVIVAL_API UISForgeCraftingInformation : public UObject
{
	GENERATED_BODY()
	
};
