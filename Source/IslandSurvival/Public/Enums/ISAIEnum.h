// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISAIEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EAIState : uint8
{
	Passive = 0 UMETA(DisplayName = "Passive"),  //被动
	Attacking = 1 UMETA(DisplayName = "Attacking"),  //攻击
	Escape = 2 UMETA(DisplayName = "Escape"), //逃跑
};
UENUM(BlueprintType)
enum EAISpeed : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"), //待机
	Walking = 1 UMETA(DisplayName = "Walking"), //步行
	Running = 2 UMETA(DisplayName = "Running"), //跑
	Sprinting = 3 UMETA(DisplayName = "Sprinting"), //冲刺
};
UCLASS()
class ISLANDSURVIVAL_API UISAIEnum : public UObject
{
	GENERATED_BODY()
	
};
