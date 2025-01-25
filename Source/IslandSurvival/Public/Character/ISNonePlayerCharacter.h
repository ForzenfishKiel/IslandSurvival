// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActorComponents/ISTradingSystemComponent.h"
#include "Character/ISCharacterBase.h"
#include "Interface/ISNPCInterface.h"
#include "ISNonePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISNonePlayerCharacter : public AISCharacterBase,public IISNPCInterface,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AISNonePlayerCharacter();
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UISTradingSystemComponent> NPCTradComponent; //NPC的可交易组件

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> NPCAsc;

	//自身的ISNPCInterface接口
	virtual AISNonePlayerCharacter* GetNPC_Implementation() override;
	virtual void OnNPCWasInteracted_Implementation(AActor* InteractingActor) override;  //当NPC被交互时
private:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> NPCTradInComingCoinEffect; //当角色向NPC购买物品时，NPC将会发送这个Effect扣除角色一定的金币
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> NPCTradInRecoverCoinEffect;  //当角色向NPC出售物品时，NPC僵毁发送这个Effect增加角色一定的金币
};
