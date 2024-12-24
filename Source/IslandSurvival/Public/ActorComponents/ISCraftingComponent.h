// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "ISCraftingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UISCraftingComponent();
	UFUNCTION(BlueprintCallable)
	int32 FindCheckCharacterBackPack(const UDataTable*TargetDT,const int32 TargetID,const int32 RequireID);
	UFUNCTION(BlueprintCallable)
	bool IsCanBeCrafting(const UDataTable*TargetDT,const int32 TargetID,const int32 RequireID);  //检查是否可以制造
	UFUNCTION(BlueprintCallable)
	void CraftingAction(const UDataTable*TargetDT,const int32 TargetID);  //制造开始
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void AddToCharacterBackPack(const UDataTable*TargetDT,const int32 TargetID);  //添加到角色背包
	UFUNCTION(Server,Reliable)
	void SendXPToTarget(float TargetXP);  //往对方传输经验值
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
