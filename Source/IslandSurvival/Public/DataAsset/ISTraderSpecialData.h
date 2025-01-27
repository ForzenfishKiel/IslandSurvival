// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ISTraderSpecialData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTraderSalesData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> TradNumCurveTable;  //交易的数量表
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> TradeSalesCurveTable;  //交易的价格表
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable> CanTradeCurveTable; //可出售的物品表
};
UCLASS()
class ISLANDSURVIVAL_API UISTraderSpecialData : public UDataAsset
{
	GENERATED_BODY()
public:
	FTraderSalesData GetTraderSalesData(const FName InName) const;
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName , FTraderSalesData> TraderSalesDataMap; //根据对应的商人名称获取其对应的交易表
};
