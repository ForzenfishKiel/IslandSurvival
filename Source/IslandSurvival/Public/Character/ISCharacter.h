// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISCharacterInventory.h"
#include "ActorComponents/ISCraftingComponent.h"
#include "ActorComponents/ISEquipmentComponent.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "ActorComponents/ISInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/ISCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/ISPlayerInterface.h"
#include "ISCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISCharacter : public AISCharacterBase,public IISPlayerInterface
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
	virtual AISPlayerState*GetPlayerState_Implementation() override;

	UFUNCTION(BlueprintCallable)
	bool CheckIsFastRun();

	UPROPERTY(EditAnywhere,Category = "Config")
	TSubclassOf<UGameplayEffect>PlayerDefaultAttribute;
	UPROPERTY(EditAnywhere,Category = "Config")
	TSubclassOf<UGameplayEffect>PlayerSecondaryAttribute;
	
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

	UPROPERTY(BlueprintReadWrite,Replicated,Category = "Config")
	float CharacterSpeed = 400.f;
private:
	void InitAbilityActorInfo();
	void InitializePlayerAttribute(UAbilitySystemComponent* ASC,TSubclassOf<UGameplayEffect>AttributeClass);
	void AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>&TargetActivateAbilities);
	void AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>&TargetActivateAbilities);
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void AddAttributeLevel(const FGameplayAttribute TargetPointType);
	UPROPERTY(EditAnywhere,Category = "Config")
	TArray<TSubclassOf<UGameplayAbility>> CharacterActivateAbilities;
	UPROPERTY(EditAnywhere,Category = "Config")
	TArray<TSubclassOf<UGameplayAbility>> CharacterPassiveAbilities;  //被动技能序列
};
