// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ISEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

AISAIController::AISAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(Blackboard);  //断言查看黑板是否成功添加
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);  //断言行为树组件是否成功添加
}

void AISAIController::SetSpeed(const EAISpeed InSpeed)
{
	AISEnemy* SourcePawn = Cast<AISEnemy>(GetPawn());
	if(!SourcePawn) return;

	SourcePawn->SetAISpeed(InSpeed);
}

void AISAIController::SetAIState(const EAIState InAIState)
{
	Blackboard->SetValueAsEnum("State", InAIState);  //设置黑板上的键为某一值
}
