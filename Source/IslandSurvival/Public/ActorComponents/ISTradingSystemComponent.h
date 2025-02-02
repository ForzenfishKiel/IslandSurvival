// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISTradingSystemComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTradingSucceefully,FItemInformation,TargetItem,int32,TargetNums);
UCLASS()
class ISLANDSURVIVAL_API UISTradingSystemComponent : public UISItemsContainer
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void TradBegin(AActor* TargetActor, FName InTargetID, int32 TargetIndex);  //角色开始交易，导入交易对象
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SaleBegin(AActor* TargetActor, FName InTargetID, int32 TargetIndex);  //角色开始出售，导入出售对象
	UFUNCTION(BlueprintCallable)
	bool CheckIsCanBeSale(AActor* TargetActor, FName InTargetID, int32 TargetIndex);  //检查当前物品是否是可以出售的

	
	UFUNCTION(Server,Reliable)
	void SetTradTarget(const FItemInformation TargetItem,const int32 TargetNums);
	UPROPERTY(Replicated)
	FItemInformation TradTarget;
	UPROPERTY(Replicated)
	int32 TargetCoins;
	UFUNCTION(Client,Reliable)
	void OnRep_TraCoins();

	UPROPERTY(BlueprintAssignable)
	FOnTradingSucceefully OnTradingSucceeded;
protected:
	void LoadingTradBackPack();
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> NPCTradInComingCoinEffect; //当角色向NPC购买物品时，NPC将会发送这个Effect扣除角色一定的金币
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> NPCTradInRecoverCoinEffect;  //当角色向NPC出售物品时，NPC将会发送这个Effect增加角色一定的金币
};
