// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISCraftingComponent.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISStroageContainer.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISStroageContainer : public UISItemsContainer
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ISStroageContainer")
	void CraftAction(UISCraftingComponent* TargetCraftingComponent,const UDataTable* TargetDT,const int32 TargetID);
protected:
	virtual void BeginPlay() override;
};
