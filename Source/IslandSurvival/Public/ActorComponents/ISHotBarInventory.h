// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISHotBarInventory.generated.h"

/**
 *   角色物品栏/快捷栏
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryUpdateAmmos,int32,CurrentAmmo,int32,MaxAmmo,int32,TargetIndex);
UCLASS()
class ISLANDSURVIVAL_API UISHotBarInventory : public UISItemsContainer
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnInventoryUpdateAmmos OnInventoryUpdateCurrentAmmos;
	
	virtual void BeginPlay() override;
	void UpdateAmmos(const int32 InCurrentAmmo,const int32 InMaxAmmo);
};
