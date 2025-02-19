// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask/WaitCharacterSpeedChangeToZero.h"

#include "Character/ISCharacter.h"

UWaitCharacterSpeedChangeToZero* UWaitCharacterSpeedChangeToZero::DetectSpeedChange(UGameplayAbility* OwingAbility)
{
	UWaitCharacterSpeedChangeToZero*MyTask = NewAbilityTask<UWaitCharacterSpeedChangeToZero>(OwingAbility);
	MyTask->bTickingTask = true;  //Tick任务检测打开
	return MyTask;
}

void UWaitCharacterSpeedChangeToZero::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	AISCharacter*ISCharacter = Cast<AISCharacter>(GetAvatarActor());
	if(ISCharacter&&ISCharacter->GetCharacterMovement())
	{
		if(!ISCharacter->CheckIsFastRun())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("STOP"));
			OnSpeedChange.Broadcast();
		}
	}
}
