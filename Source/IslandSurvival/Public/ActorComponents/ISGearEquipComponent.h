// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISGearEquipComponent.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISGearEquipComponent : public UISItemsContainer
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void InitializeEquipContainer();
};
