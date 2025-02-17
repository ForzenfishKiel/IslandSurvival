// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Interface/ISContainerInterface.h"
#include "ISItemsContainer.generated.h"
class UISInventorySystem;
class UISGearEquipComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWhenItemPickUp,FItemInformation,TargetItemInfo,int32,TargetIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPickUpOnUI,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemDiscard,int32,TargetIndex,int32,TargetQuantity);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISItemsContainer : public UActorComponent,public IISContainerInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="config")
	FName ContainerName;  //背包名字
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="config")
	int32 InventorySpace = 0;  //背包空间
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="config")
	bool bReplicated = false;  //是否复制？
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 LastChooseIndex = -1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CurrentChooseIndex = -1;
	
	FItemInformation ItemInfo;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EContainerType ContainerType = EContainerType::None;
	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnInventoryUpdate InventoryUpdate;
	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FWhenItemPickUp ItemPickup;
	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FItemPickUpOnUI ItemPickupOnUI;
	
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnItemDiscard ItemDiscard;
	
	UISItemsContainer();
	void WhenInventoryChange(UISItemsContainer*TargetContainer,const int32 TargetIndex);
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void WhenItemExchanged(UISItemsContainer*TargetItemsContainer,const int32 SourceIndex,const int32 TargetIndex);
	
	bool CheckGearSlotExchanged(UISItemsContainer*TargetGear,const int32 TargetIndex,const int32 SourceIndex);
	
	UFUNCTION(Server,Reliable)
	void ToPickUpItemsInBackPack(const FItemInformation Information, int32 TargetQuantity);  //拾取物品函数，在客户端上运行
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void DiscardItem(const int32 TargetIndex,const int32 TargetQuantity);  //丢弃物品

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void DiscardItemFromID(const int32 TargetID , const int32 TargetQuantity); //丢弃物品 
	
	UFUNCTION()
	void InitializeContainerSpace(const int32 Space);
	
	bool CheckInventoryEmpty(const FItemInformation Information);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_ContainerChange,SaveGame)
	TArray<FItemInformation>InventoryContainer;//背包

	UFUNCTION()
	void OnRep_ContainerChange();
	
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual EContainerType GetTargetContainerType() override;
	
	virtual FName GetContainerName() override;
	
	void PickUpItemForActor(APawn* TargetPawn, AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable,Client,Reliable)
	void PickUpItemForID(APawn*TargetPawn,FName TargetID,const int32 TargetNums);

	UISInventorySystem* FindInventoryByName(const FName InName);
	
	TSet<TWeakObjectPtr<AActor>> RepInventoryPlayer;  //保存操控网络对象的数组
private:
	UPROPERTY()
	TArray<UISInventorySystem*> RepInventories; //储存复制的背包Map
	virtual void InitializeComponent() override;
	virtual bool CheckConnection(UActorChannel* Channel);
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
