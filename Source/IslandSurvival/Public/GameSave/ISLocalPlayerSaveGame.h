// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ISPlayerItemDataTable.h"
#include "GameFramework/SaveGame.h"
#include "ISLocalPlayerSaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSaveAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> ISGameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "ClassDefaults")
	int32 AbilityLevel = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	//当前技能的状态标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	//当前技能装配到的插槽，如果技能未装配则为空
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityInputTag = FGameplayTag();

	//当前技能的类型（主动技能还是被动技能）
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();
};
//自定义运算符==，如果左右都是FSavedAbility类型的值，将通过函数内的值判断是否相等。
inline bool operator == (const FSaveAbility& Left, const FSaveAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}
USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	//Actor身上序列号的数据，必须通过UPROPERTY定义过，只在保存存档时使用。
	UPROPERTY()
	TArray<uint8> Bytes;
};

//自定义运算符==，如果结构体内的ActorName相同，这代表这两个结构体为相同结构体
inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

//地图相关数据保存
USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();  //当前地图的资产名字

	UPROPERTY()
	TArray<FSavedActor> SavedActors;  //当前世界中保存的Actors
};
UCLASS()
class ISLANDSURVIVAL_API UISLocalPlayerSaveGame : public ULocalPlayerSaveGame
{
	GENERATED_BODY()
public:
	

	//存档名称
	UPROPERTY()
	FString SlotName = FString();

	//存档索引
	int32 SlotIndex = 0;

	//玩家名称
	UPROPERTY()
	FString PlayerName = FString("Default Name");

	/************************** 玩家相关数据 **************************/

	//角色等级
	UPROPERTY()
	int32 PlayerLevel = 1;

	//经验值
	UPROPERTY()
	int32 XP = 0;
	
	//可分配属性点
	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	FTransform PlayerTransform;  //玩家位置

	/************************** 主要属性 **************************/

	UPROPERTY()
	float Health = 0.0f;

	UPROPERTY()
	float Hungry = 0.0f;

	UPROPERTY()
	float Thirsty = 0.0f;

	UPROPERTY()
	float Vigor = 0.0f;

	/************************** 其他数据 **************************/

	UPROPERTY()
	TArray<FItemInformation> Items;  //玩家背包
	UPROPERTY()
	TArray<FItemInformation> HotBarItems; //玩家物品栏
	UPROPERTY()
	TArray<FSaveAbility> SaveAbilities;  //保存的技能配置序列

	/************************保存地图相关*****************************/
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	//通过地图名称获取地图数据
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);

	//判断存档是否含有对于地图数据
	bool HasMap(const FString& InMapName);

};
