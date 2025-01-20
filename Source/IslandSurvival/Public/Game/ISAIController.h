// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enums/ISAIEnum.h"
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
protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;  //添加行为树组件
};
