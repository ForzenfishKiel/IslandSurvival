// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enums/ISAIEnum.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "ISAIController.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISAIController : public AAIController
{
	GENERATED_BODY()
public:
	AISAIController();
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKeySelector;
	UFUNCTION(BlueprintCallable)
	void SetSpeed(const EAISpeed InSpeed);
	UFUNCTION(BlueprintCallable)
	void SetAIState(const EAIState InAIState);
	UFUNCTION(BlueprintCallable)
	FAIStimulus GetPerceptionInfo(const EAISense InAISense);
protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;  //添加行为树组件
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerception;  //AI感知组件添加
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAISenseConfig_Sight> AISightSense;  //AI的看组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated)
	FActorPerceptionBlueprintInfo ActorPerceptionInfo;
private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
