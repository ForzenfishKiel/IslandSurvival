// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISWeaponDataAsset.h"

FWeaponInfo UISWeaponDataAsset::GetWeaponInfo(const int32 TargetID)
{
	return WeaponInfoMap.FindChecked(TargetID);
}
