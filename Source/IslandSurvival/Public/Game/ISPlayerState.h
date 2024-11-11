// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DataAsset/ISLevelUpInformation.h"
#include "ISPlayerState.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UAttributeSet;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,int32);
UCLASS()
class ISLANDSURVIVAL_API AISPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AISPlayerState();
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UISLevelUpInformation> ISLevelUpInformation;  //导入角色经验值数据表
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet*GetAttributeSet() const{return ISAttributeSet;}
	
	FOnPlayerStateChanged OnPlayerLevelChanged;
	FOnPlayerStateChanged OnPlayerXPChange;
	FOnPlayerStateChanged OnPlayerAttributePointChange;  //属性点发生变化时

	FORCEINLINE int32 GetPlayerLevel() const {return CurrentLevel;}  //获取角色当前的等级
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);

	FORCEINLINE int32 GetXP() const {return CurrentXP;}
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	FORCEINLINE int32 GetAttributePoint() const {return AttributePoint;}
	void AddToAttributePoint(int32 InAttributePoint);
	void SetAttributePoint(int32 InAttributePoint);
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ISAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet>ISAttributeSet;


private:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentXP = 0;
	UPROPERTY(VisibleAnywhere)
	int32 CurrentLevel = 1;
	UPROPERTY(VisibleAnywhere)
	int32 AttributePoint = 0;
};
