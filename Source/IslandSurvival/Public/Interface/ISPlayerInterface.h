// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ISPlayerController.h"
#include "Game/ISPlayerState.h"
#include "UObject/Interface.h"
#include "ISPlayerInterface.generated.h"

class AISCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISPlayerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddToXP(int32 XP);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 FindLevelFromXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetXP();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetLevel();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetAttributePointsReward(int32 Level) const;  //从等级中获取当前技能点

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetSpellPointsReward(int32 Level) const; //从等级中获取当前技能点

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddToPlayerLevel(int32 InPlayerLevel);  //增加等级

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddToAttributePoints(int32 InAttributePoints);//增加属性点

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddToSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void LevelUp();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AISPlayerState*GetSourcePlayerState();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AISCharacter* GetSourceCharacter();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SaveProgress();  //保存玩家状态

	//获取角色使用的次级属性GameplayEffect
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UGameplayEffect> GetSecondaryAttributes();

	//获取角色使用的额外属性GameplayEffect
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UGameplayEffect> GetPrimaryAttributes();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FString GetPlayerName() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPlayerRespawnLocation(const FVector& InLocation);
};
