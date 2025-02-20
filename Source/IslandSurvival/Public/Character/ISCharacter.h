// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISBuildingComponent.h"
#include "ActorComponents/ISCharacterInventory.h"
#include "ActorComponents/ISCraftingComponent.h"
#include "ActorComponents/ISEquipmentComponent.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "ActorComponents/ISInteractionComponent.h"
#include "ActorComponents/ISTradingSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/ISCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/ISPlayerInterface.h"
#include "UI/ISMenuUIBase.h"
#include "ISCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISCharacter : public AISCharacterBase,public IISPlayerInterface,public IISPublicInterface
{
	GENERATED_BODY()
public:
	AISCharacter();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;  //设定服务器同步数据，也是角色控制器调用时调用
	virtual void OnRep_PlayerState() override;  //客户端同步玩家数据
	virtual void AddToXP_Implementation(int32 XP) override;
	virtual int32 FindLevelFromXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 GetLevel_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;  //获取奖励的属性点
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;  //获取奖励的技能点
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void LevelUp_Implementation() override;  //角色升级，播放粒子特效或音效等
	virtual AISPlayerState*GetSourcePlayerState_Implementation() override;
	virtual void BindAttributeSet() const override;
	virtual void ApplyDamageToTarget_Implementation(AActor* Target) override;
	virtual AISCharacter* GetSourceCharacter_Implementation() override;
	virtual void Die() override;
	virtual void MulticastHandleDeath() override;
	virtual UISMenuUIBase* GetMenuUI_Implementation() const override;
	virtual void SaveProgress_Implementation() override;
	virtual TSubclassOf<UGameplayEffect> GetSecondaryAttributes_Implementation() override;
	virtual TSubclassOf<UGameplayEffect> GetPrimaryAttributes_Implementation() override;
	virtual FString GetPlayerName_Implementation() const override;
	virtual void SetPlayerRespawnLocation_Implementation(const FVector& InLocation) override;
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SetOwnerWhenCharacterControlActor(const TArray<UISItemsContainer*>&InItemsContainer, APlayerController* InController);
	UFUNCTION(Server,Reliable)
	void SetSingleOwnerWhenCharacterControlActor(const UISItemsContainer* InItemsContainer,APlayerController* InController);
	void SetPlayerName(const FString& InPlayerName);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AISHarvestingBase> CheckHarvestDataAsset(const FString& InName);
	UFUNCTION(BlueprintCallable)
	AISHarvestingBase* ReplaceToActor(const TSubclassOf<AISHarvestingBase> InClassTemp,
		UInstancedStaticMeshComponent* TargetComponent,const int32 TargetItemNum);
	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void MulticastToRemoveStaticMesh(UInstancedStaticMeshComponent* TargetComponent,const int32 TargetItemNum);

	UFUNCTION(Client,Reliable)
	void TradWindowOpen(AActor* TargetActor);
	UFUNCTION(Client,Reliable)
	void MenuWindowOpen(AActor* TargetActor);
	UFUNCTION(Client,Reliable)
	void TradWindowClose(APlayerController* TargetController);

	
	UFUNCTION(BlueprintCallable)
	bool CheckIsFastRun();

		
	// 网络同步函数
	UFUNCTION(BlueprintCallable)
	void PlayBGM(EBGMType Type, float FadeTime);

	UFUNCTION(BlueprintCallable)
	void StopBGM(EBGMType Type, float FadeTime);


	UPROPERTY(BlueprintReadOnly,Replicated)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere,Category = "Config")
	TSubclassOf<UGameplayEffect>PlayerDefaultAttribute;
	UPROPERTY(EditAnywhere,Category = "Config")
	TSubclassOf<UGameplayEffect>PlayerSecondaryAttribute;

	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UISBGMManagerComponent> BGMManager;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISCharacterInventory>CharacterInventory;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISHotBarInventory>CharacterHotBarInventory;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISEquipmentComponent>CharacterEquipment;  //角色装备组件
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISInteractionComponent> ISInteractionComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISCraftingComponent>ISCraftingComponent;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Combine")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UISGearEquipComponent> GearEquipComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISBuildingComponent>ISBuildingComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISTradingSystemComponent> ISTradingSystemComponent;

	UPROPERTY()
	TObjectPtr<UISMenuUIBase> MenuUIRef;

	UPROPERTY(BlueprintReadWrite,Replicated,Category = "Config")
	float CharacterSpeed = 400.f;
private:
	
	virtual void InitAbilityActorInfo() override;
	
	virtual void InitializePlayerAttribute(UAbilitySystemComponent* ASC,TSubclassOf<UGameplayEffect>AttributeClass) override;
	
	virtual void AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>&TargetActivateAbilities) override;
	
	virtual void AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>&TargetActivateAbilities) override;
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void AddAttributeLevel(const FGameplayAttribute TargetPointType);
	UFUNCTION(Client,Reliable)
	void ClearPlayerMainHUD();
	void LoadProgress(); //加载进度
	
	FVector GetPlayerRespawnLocation() const;
	
	UPROPERTY(EditAnywhere,Category = "Config")
	TArray<TSubclassOf<UGameplayAbility>> CharacterActivateAbilities;
	UPROPERTY(EditAnywhere,Category = "Config")
	TArray<TSubclassOf<UGameplayAbility>> CharacterPassiveAbilities;  //被动技能序列
};
