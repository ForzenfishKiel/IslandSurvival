// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "ISCharacterClassInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category = "CharacterConfig")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditDefaultsOnly,Category = "CharacterConfig")
	FScalableFloat XPReward = FScalableFloat();  //设置敌人死亡后传入的经验值

	UPROPERTY(EditDefaultsOnly,Category = "CharacterConfig")
	TArray<TSubclassOf<UGameplayAbility>> StartupActivateAbilities;  //敌人的主动技能

	UPROPERTY(EditDefaultsOnly,Category = "CharacterConfig")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;  //敌人的被动技能
};
UCLASS()
class ISLANDSURVIVAL_API UISCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category = "CharacterConfig")
	TMap<FName,FCharacterClassDefaultInfo> CharacterClassInfomation;  //根据传入的名字获取对应的角色配置

	UPROPERTY(EditDefaultsOnly,Category = "CharacterConfig")
	TSubclassOf<UGameplayEffect> SecondaryAttribute;

	//主要属性，玩家的基础属性，通过SetByCaller设置
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	FCharacterClassDefaultInfo GetCharacterClassData(const FName InTargetName);
};
