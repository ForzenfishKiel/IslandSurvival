// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISLevelUpInformation.h"

int32 UISLevelUpInformation::FindLevelFromXP(int32 InComingXP)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if(LevelUpArray.Num() - 1<=Level) return Level;  //数组0号位不记录等级
		if(InComingXP>=LevelUpArray[Level].LevelUpRequirement)  //如果经验值刚好大于等于升级所需经验值，则等级+1
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
