// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISTradingSystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISTradingSystemComponent : public UISItemsContainer
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void TradBegin(AActor* TargetActor, FName InTargetID);  //角色开始交易，导入交易对象
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SaleBegin(AActor* TargetActor, FName InTargetID);  //角色开始出售，导入出售对象
protected:
	void LoadingTradBackPack();
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> NPCTradInComingCoinEffect; //当角色向NPC购买物品时，NPC将会发送这个Effect扣除角色一定的金币
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> NPCTradInRecoverCoinEffect;  //当角色向NPC出售物品时，NPC将会发送这个Effect增加角色一定的金币
};
