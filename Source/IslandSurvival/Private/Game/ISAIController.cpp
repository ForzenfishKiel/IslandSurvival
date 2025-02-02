// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ISEnemy.h"
#include "Interface/ISPlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"

AISAIController::AISAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(Blackboard);  //断言查看黑板是否成功添加
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);  //断言行为树组件是否成功添加

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	if(AISightSense)
	{
		AISightSense->PeripheralVisionAngleDegrees = 60.f; //设置AI的看的锥形角度为60
		AISightSense->DetectionByAffiliation.bDetectEnemies = true;
		AISightSense->DetectionByAffiliation.bDetectFriendlies = true;
		AISightSense->DetectionByAffiliation.bDetectNeutrals = true;
	}
	AIPerception->OnPerceptionUpdated.AddDynamic(this,&AISAIController::OnPerceptionUpdated);  //初始化绑定AI的感知变化
}

void AISAIController::SetSpeed(const EAISpeed InSpeed)
{
	AISEnemy* SourcePawn = Cast<AISEnemy>(GetPawn());
	if(!SourcePawn) return;

	SourcePawn->SetAISpeed(InSpeed);
}

void AISAIController::SetAIState(const EAIState InAIState)
{
	IISPublicInterface::Execute_SetAIState(GetPawn(), InAIState);  //设置AI的状态
	Blackboard->SetValueAsEnum(FName("State"),InAIState);
}


//当AI感知到其他玩家的时候
void AISAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if(UpdatedActors.IsEmpty()) return;  //为看到任何生物则返回

	for(AActor* ActorRef : UpdatedActors)
	{
		if(!ActorRef->Implements<UISPlayerInterface>()) return;  //如果看到的不是玩家类则返回
		if(AIPerception->GetActorsPerception(ActorRef, ActorPerceptionInfo))
		{
			
			Blackboard->SetValueAsObject(FName("TargetActor"),ActorRef);  //设置黑板的键值
		}
	}
}

FAIStimulus AISAIController::GetPerceptionInfo(const EAISense InAISense)
{
	if(!&ActorPerceptionInfo) return FAIStimulus();
	for(const FAIStimulus AIStimulus : ActorPerceptionInfo.LastSensedStimuli)
	{
		TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(this,AIStimulus);
		
		if(InAISense == EAISense::Sighting && Sense.Get() == UAISense_Sight::StaticClass())
		{
			return AIStimulus;
		}
		if(InAISense == EAISense::TakeDamage && Sense.Get() == UAISense_Damage::StaticClass())
		{
			return AIStimulus;
		}
	}
	return FAIStimulus();
}

void AISAIController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(AISAIController,ActorPerceptionInfo,SharedParams);
}
