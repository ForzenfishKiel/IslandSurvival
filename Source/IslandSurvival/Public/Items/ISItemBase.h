// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/ISItemInterface.h"
#include "ISItemBase.generated.h"
//物品种类分配
UENUM(BlueprintType)
enum class EItemType:uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Resource = 1 UMETA(DisplayName = "Resource"),  //资源
	Equipable = 2 UMETA(DisplayName = "Equipable"),  //可装备
	Armor = 3 UMETA(DisplayName = "Armor"), //护甲
	Consumable = 4 UMETA(DisplayName = "Consumable"), //消耗品
	Buildable = 5 UMETA(DisplayName = "Buildable"),  //建筑类
};
UENUM(BlueprintType)
enum class EItemRarity:uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Common = 1 UMETA(DisplayName = "Common"), //常见的
	UnCommon = 2 UMETA(DisplayName = "UCommon"), //不常见的
	Rare = 3 UMETA(DisplayName = "Rare"),  //罕见的
	Epic = 4 UMETA(DisplayName = "Epic"), //史诗的
	Legendary = 5 UMETA(DisplayName = "Legendary"),  //传奇的
};
UENUM(BlueprintType)
enum class EArmorType:uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Helmet = 1 UMETA(DisplayName = "Helmatic"),  //头盔
	Chest = 2 UMETA(DisplayName = "Chest"), //衣服
	Pants = 3 UMETA(DisplayName = "Pants"),  //裤子
	Boots = 4 UMETA(DisplayName = "Boots"),  //鞋子
};
UCLASS()
class ISLANDSURVIVAL_API AISItemBase : public AActor,public IISItemInterface
{
	GENERATED_BODY()

public:	
	AISItemBase();
	UPROPERTY(EditAnywhere,Category = "ItemConfig")
	FName ItemID;
	UPROPERTY(EditAnywhere,Category = "ItemConfig")
	EItemType ItemType = EItemType::None;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ItemsStaticMesh;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent>PickUpCheckSphere;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PickUpItemToInventory_Implementation(APawn* TargetPawn, AActor* TargetActor) override;
	virtual void UseItem(AActor*TargetCharacter,UAbilitySystemComponent*TargetASC);  //使用物品和不使用物品都要在服务器上运行
	virtual void UnUseItem(AActor*TargetCharacter,UAbilitySystemComponent*TargetASC);
	virtual void ApplyEffectToTarget(UAbilitySystemComponent*InASC,TSubclassOf<UGameplayEffect>EffectClass){return;}
	virtual void AddTargetAbility(UAbilitySystemComponent*TargetASC,TArray<TSubclassOf<UGameplayAbility>>&TargetArray){return;}
	virtual void RemoveTargetAbility(UAbilitySystemComponent*TargetASC,TArray<TSubclassOf<UGameplayAbility>>&TargetArray){return;}
	virtual void RemoveTargetEffect(UAbilitySystemComponent*TargetASC,TSubclassOf<UGameplayEffect>EffectClass){return;}
};
