// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ISCharacterBase.h"
#include "Components/ActorComponent.h"
#include "Items/ISEquipable.h"
#include "ISEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquip,TSubclassOf<AISItemBase>,TargetItemClass);
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
	UPROPERTY(BlueprintReadOnly)
	ECharacterEquipState CharacterEquipState = ECharacterEquipState::None;
	UPROPERTY()
	TObjectPtr<AISEquipable> Equipable = nullptr;//用于储存角色可装备的物品
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC;
public:
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Equip(TSubclassOf<AISItemBase> InTargetItem);
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void UnEquip();
	void InitializeEquipmentComponent(UAbilitySystemComponent*TargetASC);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
