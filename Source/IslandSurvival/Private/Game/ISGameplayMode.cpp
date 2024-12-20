// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISGameplayMode.h"

void AISGameplayMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	LoginPlayerList.Add(NewPlayer);  //当客户端登录服务器时，向数组导入
}

APlayerController* AISGameplayMode::FindPlayerControllerFromLocalPlayer(ULocalPlayer* InLocalPlayer)
{
	if(LoginPlayerList.Contains(InLocalPlayer->GetPlayerController(GetWorld())))
	{
		return InLocalPlayer->GetPlayerController(GetWorld());
	}
	return nullptr;
}
