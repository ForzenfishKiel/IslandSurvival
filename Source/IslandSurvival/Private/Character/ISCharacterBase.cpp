// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacterBase.h"

AISCharacterBase::AISCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AISCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
void AISCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AISCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

