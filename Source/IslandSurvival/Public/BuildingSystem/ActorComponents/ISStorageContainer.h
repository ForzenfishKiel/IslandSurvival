// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "UI/ISMenuUIBase.h"
#include "ISStorageContainer.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISStorageContainer : public UISItemsContainer
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
