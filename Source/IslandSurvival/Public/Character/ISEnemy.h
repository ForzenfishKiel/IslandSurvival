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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHealthChangeSignature,float,NewValue,float,NewMaxValue);  //绑定玩家属性变化
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

	UPROPERTY(EditDefaultsOnly,Replicated,BlueprintReadOnly, Category="Config")
	TEnumAsByte<EAIState> AIState;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnEnemyHealthChangeSignature OnHealthChange;
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//ICombatInterface 战斗接口
	virtual int32 GetLevel_Implementation() override;
	virtual FGameplayAbilitySpecHandle FindActivateAbility_Implementation(const FGameplayTag InTag) const override;
	virtual void ApplyDamageToTarget_Implementation(AActor* Target) override;
	//IEnemyInterface 接口
	virtual AISAIController* GetAIController_Implementation() override;
	virtual EAIState GetAIState_Implementation() const override;
	virtual void SetAIState_Implementation(EAIState State) override;
	virtual AISEnemy* GetEnemy_Implementation() override;
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EAISpeed>,float> AISpeedManager;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	float MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth();
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health;
	UFUNCTION()
	void OnRep_Health();
};
