// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISHotBarInventory.generated.h"

/**
 *   角色物品栏/快捷栏
 */
UCLASS()
class ISLANDSURVIVAL_API UISHotBarInventory : public UISItemsContainer
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
