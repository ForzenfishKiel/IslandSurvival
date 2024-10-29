// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISCharacterInventory.h"
#include "ActorComponents/ISEquipmentComponent.h"
#include "ActorComponents/ISHotBarInventory.h"
#include "ActorComponents/ISInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/ISCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "ISCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISCharacter : public AISCharacterBase
{
	GENERATED_BODY()
public:
	AISCharacter();
	virtual void PossessedBy(AController* NewController) override;  //设定服务器同步数据，也是角色控制器调用时调用
	virtual void OnRep_PlayerState() override;  //客户端同步玩家数据

	UPROPERTY(EditAnywhere,Category = "Attributes")
	TSubclassOf<UGameplayEffect>PlayerDefaultAttribute;
	UPROPERTY(EditAnywhere,Category = "Attributes")
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
	UPROPERTY()
	TObjectPtr<UISInteractionComponent> ISInteractionComponent;
	UPROPERTY(EditAnywhere,Category = "Combine")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;
protected:

private:
	void InitAbilityActorInfo();
	void InitializePlayerAttribute(UAbilitySystemComponent* ASC,TSubclassOf<UGameplayEffect>AttributeClass);
};
