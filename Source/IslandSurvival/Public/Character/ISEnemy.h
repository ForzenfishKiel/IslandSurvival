// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/ISCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Game/ISAIController.h"
#include "Interface/ISEnemyInterface.h"
#include "WidgetController/ISMainUIWidgetController.h"
#include "ISEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISEnemy : public AISCharacterBase,public IAbilitySystemInterface,public IISEnemyInterface
{
	GENERATED_BODY()
	AISEnemy();
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ISEnemyAbilitysystem;
	UPROPERTY()
	TObjectPtr<UAttributeSet> ISEnemyAttribute;
	UPROPERTY()
	TObjectPtr<AISAIController> ISEnemyAIController;
	UPROPERTY(EditAnywhere, Category="Config")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	int32 Level = 1.f;  //敌人的等级
public:
	UPROPERTY(BlueprintAssignable) 
	FOnPlayerStateChangeSignature OnAttributeChange;  //原始数值变化
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangeSignature OnMaxAttributeChange;  //最大数值变化
	void SetAISpeed(const EAISpeed InState);
public:
	//角色自身虚函数
	virtual void Die() override;
	virtual void MulticastHandleDeath() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	UAttributeSet* GetEnemyAttribute() const;
private:
	virtual void InitAbilityActorInfo() override;
	virtual void InitializePlayerAttribute(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> AttributeClass) override;
	//ICombatInterface 战斗接口
	virtual int32 GetLevel_Implementation() override;
	//IEnemyInterface 接口
	virtual AISAIController* GetAIController_Implementation() override;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EAISpeed>,float> AISpeedManager;
};
