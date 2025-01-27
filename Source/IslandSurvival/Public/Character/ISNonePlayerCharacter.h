// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActorComponents/ISTradingSystemComponent.h"
#include "Character/ISCharacterBase.h"
#include "Game/ISPlayerController.h"
#include "Interface/ISNPCInterface.h"
#include "UI/ISMenuUIBase.h"
#include "ISNonePlayerCharacter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetAttributeChange,float,NewValue);
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

	UPROPERTY(BlueprintAssignable)
	FOnTargetAttributeChange OnTargetAttributeChange;

	UFUNCTION(Client,Reliable)
	void LoadingTradWindow();
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
private:
	UPROPERTY(ReplicatedUsing = OnRep_Actor)
	TObjectPtr<AActor>TraderUserWidgetActor;
	UFUNCTION()
	void OnRep_Actor();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISMenuUIBase> TraderUserWidgetClass;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	UFUNCTION(Client,Reliable)
	void TradWindowOpen(AActor* TargetActor);
	UFUNCTION(Client,Reliable)
	void TradWindowClose(APlayerController* TargetController);
	
};
