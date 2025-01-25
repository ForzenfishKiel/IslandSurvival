// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISTraderSpecialData.h"

FTraderSalesData UISTraderSpecialData::GetTraderSalesData(const FName InName) const
{
	return TraderSalesDataMap.FindChecked(InName);
}
