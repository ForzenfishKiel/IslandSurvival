// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "DataAsset/ISStorageDataConfig.h"
#include "Interface/ISContainerInterface.h"
#include "ISItemsContainer.generated.h"
class UISGearEquipComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWhenItemPickUp,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPickUpOnUI,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemDiscard,int32,TargetIndex,int32,TargetQuantity);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISItemsContainer : public UActorComponent,public IISContainerInterface
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName ContainerName;  //背包名字
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EContainerType ContainerType = EContainerType::None;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 LastChooseIndex = -1;
	FItemInformation ItemInfo;  //空 格

	UPROPERTY(BlueprintReadOnly,Replicated)
	FInventoryConfig InventoryConfig;
	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnInventoryUpdate InventoryUpdate;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FWhenItemPickUp ItemPickup;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FItemPickUpOnUI ItemPickupOnUI;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnItemDiscard ItemDiscard;

	void InitializeContainer();
	
	UISItemsContainer();
	void WhenInventoryChange(UISItemsContainer*TargetContainer,const int32 TargetIndex);  //玩家自身背包物品发生变化时
	UFUNCTION(BlueprintCallable,Client,Reliable)
	void WhenItemExchanged(UISItemsContainer*TargetItemsContainer,const int32 SourceIndex,const int32 TargetIndex);  //储存器物品发生变化时
	bool CheckGearSlotEcchanged(UISItemsContainer*TargetGear,const int32 TargetIndex,const int32 SourceIndex);
	UFUNCTION()
	void ToPickUpItemsInBackPack(const FItemInformation Information);  //拾取物品函数，在客户端上运行
	UFUNCTION(Client,Reliable)
	void DiscardItem(const int32 TargetIndex,const int32 TargetQuantity);  //丢弃物品
	void InitializeContainerSpace(const int32 Space);
	bool CheckInventoryEmpty(const FItemInformation Information);

	
	UFUNCTION(Client,Reliable)
	void CallContainerUpdateOnClient(const UISItemsContainer*TargetItemsContainer);


	/*服务器通知事件*/
	UFUNCTION(Server,Reliable)
	void SendAddDataToServer(const TArray<FItemInformation>& InItems);
	UFUNCTION(Server,Reliable)
	void SendRemoveDataToServer(const TArray<FItemInformation>& InItems);

	
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemInformation>InventoryContainer;//背包
	UPROPERTY(BlueprintReadOnly)
	TMap<EArmorType,FItemInformation> GearEquipContainer;//装备栏

	void AddRelevantPlayer(APlayerController* TargetOwner);
	void RemoveRelevantPlayer(APlayerController* TargetOwner);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<FInventoryConfig> InventoryConfigs;  //保存着所有库存的配置文件
	UPROPERTY()
	TMap<FName,UISInventorySystem*> Inventories;  //根据标签（FName）查找的对应的背包
	TSet<TWeakObjectPtr<AActor>> RelevantPlayers;  //TSet是一个不允许重复的容器，TWeakObjectPtr则意味着当AActor被销毁的时候，程序获取该对象不会获取到空，也就是获取到了悬挂指针
protected:
	virtual void BeginPlay() override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual bool CheckConnection(UActorChannel* Channel);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual EContainerType GetTargetContainerType() override;
	virtual FName GetContainerName() override;
	void PickUpItemForActor(APawn* TargetPawn, AActor* TargetActor);
	UFUNCTION(BlueprintCallable,Client,Reliable)
	void PickUpItemForID(APawn*TargetPawn,FName TargetID,const int32 TargetNums);
	virtual void OnComponentCreated() override;
private:
	UPROPERTY()
	TArray<UISInventorySystem*> RepInventories;  //管理用于复制的库存系统
};
