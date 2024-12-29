// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ISCharacterBase.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Items/ISConsumable.h"
#include "Items/ISEquipable.h"
#include "Items/ISGear.h"
#include "ISEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquip,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipGear,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnEquipGear,FItemInformation,TargetItemInfo);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UISEquipmentComponent();
	
	//Player Attach Socket Setter.
	void SetTPAttachSocket(FName NewSocketName);
	void SetFPAttachSocket(FName NewSocketName);
	
	//用于通知一些功能的实现，例如通知装备动画的播放，并传递一些数值
	UPROPERTY(BlueprintAssignable, Category="Custom Events")
	FOnEquip OnEquip;

	
	UPROPERTY(BlueprintAssignable, Category="Custom Events")
	FOnUnEquip OnUnEquip;

	
	FOnEquipGear OnEquipGear;  //装备事件

	
	FOnUnEquipGear OnUnEquipGear;  //脱下装备事件

	
	UPROPERTY(BlueprintReadOnly,Replicated)
	ECharacterEquipState CharacterEquipState = ECharacterEquipState::None;

	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_EquipableChanged)
	TObjectPtr<AISEquipable> Equipable = nullptr;//用于储存角色可装备的物品
	UFUNCTION()
	void OnRep_EquipableChanged();

	UPROPERTY()
	TObjectPtr<AISConsumable> ISConsumable = nullptr;

	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<AISGear> ISHelmet = nullptr;

	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<AISGear> ISChest = nullptr;

	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<AISGear> ISPants = nullptr;

	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<AISGear> ISBoots = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC;
public:
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Equip(const FItemInformation TargetInformation);

	
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void SpawnEquip(USceneComponent*AttachEquip);

	
	UFUNCTION(BlueprintCallable,Client, Reliable)
	void SpawnEquipOnClient();

	
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void UnEquip();

	
	UFUNCTION(BlueprintCallable,Client, Reliable)
	void UnEquipOnClient();

	
	UFUNCTION(Server, Reliable)
	void UseConsumable(TSubclassOf<AISItemBase>ItemClass);

	
	UFUNCTION(Server, Reliable)
	void UnUseConsumable();

	
	void InitializeEquipmentComponent(UAbilitySystemComponent*TargetASC);

	
	UFUNCTION(Server, Reliable)
	void EquipGear(const FItemInformation TargetInformation);

	
	UFUNCTION(server, Reliable)
	void UnEquipGear(const FItemInformation TargetInformation);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	FName Tp_AttachSocket;  //附加第三人称套接字

	UPROPERTY(Replicated)
	FName Fp_AttachSocket;  //附加第一人称套接字
};
