// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISPlayerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 XP);

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelFromXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetLevel();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;  //从等级中获取当前技能点

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const; //从等级中获取当前技能点

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);  //增加等级

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);//增加属性点

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerMaxHealthPoint();
};
