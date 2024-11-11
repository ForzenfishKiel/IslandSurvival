// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ISLevelUpInformation.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FISLevelUp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;  //到达此等级所需的经验值 
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 0;  //到达此等级奖励的属性点
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 0;  //到达此等级分配的技能点
};
UCLASS()
class ISLANDSURVIVAL_API UISLevelUpInformation : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FISLevelUp> LevelUpArray;

	int32 FindLevelFromXP(int32 InComingXP);  //从XP中查询等级
};
