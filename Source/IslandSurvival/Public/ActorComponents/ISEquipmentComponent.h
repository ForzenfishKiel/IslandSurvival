// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ISCharacterBase.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Items/ISEquipable.h"
#include "ISEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquip,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquip);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UISEquipmentComponent();

	//用于通知一些功能的实现，例如通知装备动画的播放，并传递一些数值
	UPROPERTY(BlueprintAssignable, Category="Custom Events")
	FOnEquip OnEquip;
	UPROPERTY(BlueprintAssignable, Category="Custom Events")
	FOnUnEquip OnUnEquip;
	UPROPERTY(BlueprintReadOnly,Replicated)
	ECharacterEquipState CharacterEquipState = ECharacterEquipState::None;
	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<AISEquipable> Equipable = nullptr;//用于储存角色可装备的物品
	UPROPERTY()
	TObjectPtr<AISEquipable> EquipableClient = nullptr;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC;
public:
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Equip(const FItemInformation TargetInformation);
	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void SpawnEquip(USceneComponent*AttachEquip);
	UFUNCTION(BlueprintCallable,Client, Reliable)
	void SpawnEquipOnClient(const FItemInformation TargetInformation);
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void UnEquip();
	UFUNCTION(BlueprintCallable,Client, Reliable)
	void UnEquipOnClient();
	void InitializeEquipmentComponent(UAbilitySystemComponent*TargetASC);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
