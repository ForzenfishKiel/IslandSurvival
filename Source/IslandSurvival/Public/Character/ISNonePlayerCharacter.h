// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ISPublicInterface.h"
#include "ActorComponents/ISTradingSystemComponent.h"
#include "Character/ISCharacterBase.h"
#include "Interface/ISNPCInterface.h"
#include "UI/ISMenuUIBase.h"
#include "ISNonePlayerCharacter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetAttributeChange,float,NewValue);
UCLASS()
class ISLANDSURVIVAL_API AISNonePlayerCharacter : public AISCharacterBase,public IISNPCInterface,public IAbilitySystemInterface,public
IISPublicInterface
{
	GENERATED_BODY()
public:
	AISNonePlayerCharacter();
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UISTradingSystemComponent> NPCTradComponent; //NPC的可交易组件

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> NPCAsc;

	UPROPERTY(BlueprintAssignable)
	FOnTargetAttributeChange OnTargetAttributeChange;

	UFUNCTION(Client,Reliable)
	void LoadingTradWindow();
	//公共接口PublicInterface
	virtual UISMenuUIBase* GetMenuUI_Implementation() const override;
	virtual void BindWidgetController_Implementation(AActor* TargetActor) override;
	virtual void SetAIState_Implementation(EAIState State) override;
	virtual EAIState GetAIState_Implementation() const override;
	virtual UISItemsContainer* GetItemsContainer_Implementation() const override;
	//自身的ISNPCInterface接口
	virtual AISNonePlayerCharacter* GetNPC_Implementation() override;
	virtual void OnNPCWasInteracted_Implementation(AActor* InteractingActor) override;  //当NPC被交互时
	virtual int32 GetFavorability_Implementation() const override; //获取NPC的好感度
	virtual FName GetCharacterName_Implementation() const override;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated)
	int32 Favorability = 0;

	UPROPERTY()
	TObjectPtr<UISMenuUIBase> TraderUserWidget;

	UPROPERTY(EditDefaultsOnly,Replicated,BlueprintReadOnly, Category="Config")
	TEnumAsByte<EAIState> AIState;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISMenuUIBase> TraderUserWidgetClass;
	void BindCharacterAttirbuteChange(AActor* TargetActor);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
};
