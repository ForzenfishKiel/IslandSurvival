// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "DataAsset/ISLevelUpInformation.h"
#include "ISPlayerState.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EAttributePointType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	MaxHealth = 1 UMETA(DisplayName = "MaxHealth"),
};
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
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable>PlayerAttributePointsTable;  //导入角色加点的因数值
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
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

	void AddTargetAttributeLevel(const FGameplayAttribute TargetPointType);
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ISAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet>ISAttributeSet;


private:

	/*特定属性点统计结束*/
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_XP)
	int32 CurrentXP = 0;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_Level)
	int32 CurrentLevel = 1;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoint = 0;

	//服务器出现更改自动同步到本地函数 等级
	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const; 

	//服务器出现更改自动同步到本地函数 经验值
	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;

	//服务器出现更改自动同步到本地函数 属性点
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints) const; 
};
