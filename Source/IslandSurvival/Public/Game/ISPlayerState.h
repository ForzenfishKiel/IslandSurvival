// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "ActorComponents/ISBGMManagerComponent.h"
#include "DataAsset/ISLevelUpInformation.h"
#include "ISPlayerState.generated.h"
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSendMassageEvent,FText,SendMassage,int32,PlayerID);
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
	UPROPERTY(BlueprintAssignable)
	FOnSendMassageEvent OnSendMassageEvent;
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

	UPROPERTY(ReplicatedUsing = OnRep_ChatHistory)
	FText InputMassage;

	FORCEINLINE int32 GetPlayerLevel() const {return CurrentLevel;}  //获取角色当前的等级
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);
	
	FORCEINLINE int32 GetXP() const {return CurrentXP;}
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	FORCEINLINE int32 GetAttributePoint() const {return AttributePoint;}
	void AddToAttributePoint(int32 InAttributePoint);
	void SetAttributePoint(int32 InAttributePoint);

	void SetSourcePlayerName(const FString& InSourcePlayerName);

	void AddTargetAttributeLevel(const FGameplayAttribute TargetPointType);

	FORCEINLINE FString GetSourcePlayerName() const {return PlayerName;}

	FVector GetPlayerRespawnLocation() const;
	void SetPlayerRespawnLocation(const FVector& InPlayerRespawnLocation);


	// 添加新消息（服务器调用）
	UFUNCTION(Server,Reliable,Category = "Chat")
	void AddChatMessage(const FText& InputText,const int32 InPlayerID);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UISBGMManagerComponent* BGMManager;
	
	// 网络同步函数
	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void Multicast_PlayBGM(EBGMType Type, float FadeTime);

	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void Multicast_StopBGM(EBGMType Type, float FadeTime);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ISAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet>ISAttributeSet;
private:
	FString PlayerName = FString();
	/*特定属性点统计结束*/
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_XP)
	int32 CurrentXP = 0;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_Level)
	int32 CurrentLevel = 1;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoint = 0;
	UPROPERTY(Replicated)
	int32 SourcePlayerID = -1;
	UPROPERTY()
	FVector PlayerRespawnLocation = FVector();  //角色保存的出生位置

	//服务器出现更改自动同步到本地函数 等级
	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const; 

	//服务器出现更改自动同步到本地函数 经验值
	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;

	//服务器出现更改自动同步到本地函数 属性点
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints) const;

	UFUNCTION()
	void OnRep_ChatHistory();
};
