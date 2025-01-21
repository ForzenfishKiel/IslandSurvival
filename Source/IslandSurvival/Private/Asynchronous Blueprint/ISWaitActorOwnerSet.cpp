// Fill out your copyright notice in the Description page of Project Settings.


#include "Asynchronous Blueprint/ISWaitActorOwnerSet.h"

#include "Character/ISCharacter.h"

UISWaitActorOwnerSet* UISWaitActorOwnerSet::WaitActorOwnerSet(APlayerController* InController,UISItemsContainer* TargetContainer)
{
	UISWaitActorOwnerSet* NewRequire = NewObject<UISWaitActorOwnerSet>();
	NewRequire->SourcePC = InController;
	NewRequire->SourceContainer = TargetContainer;
	return NewRequire;
}

void UISWaitActorOwnerSet::Activate()
{
	Super::Activate();

	if(SourcePC->IsLocalPlayerController())
	{
		AISCharacter*SourcePlayer = Cast<AISCharacter>(SourcePC->GetPawn());
		if(!SourcePlayer) return;
		SourcePlayer->SetSingleOwnerWhenCharacterControlActor(SourceContainer, SourcePC);
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("玩家控制器调用"));
		
		OnActorOwnerSetChanged.Broadcast(true);
	}
}