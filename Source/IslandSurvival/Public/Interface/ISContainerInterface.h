// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISContainerInterface.generated.h"

// This class does not need to be modified.
UENUM()
enum class EContainerType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Storage = 1 UMETA(DisplayName = "Storage"),  //储存箱
	HotBar = 2 UMETA(DisplayName = "HotBar"),  //快捷栏
	BackPack = 3 UMETA(DisplayName = "BackPack"), //背包
};
UINTERFACE(MinimalAPI)
class UISContainerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISContainerInterface
{
	GENERATED_BODY()
public:
	virtual EContainerType GetTargetContainerType() = 0;
};
