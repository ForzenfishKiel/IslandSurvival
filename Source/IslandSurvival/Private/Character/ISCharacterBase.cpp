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

void AISCharacterBase::ActivateCharacterAbility_Implementation()
{
	IISCombatInterface::ActivateCharacterAbility_Implementation();
}

void AISCharacterBase::EndActivateCharacterAbility_Implementation()
{
	IISCombatInterface::EndActivateCharacterAbility_Implementation();
}

void AISCharacterBase::InitializePlayerAttribute(UAbilitySystemComponent* ASC,
	TSubclassOf<UGameplayEffect> AttributeClass)
{
	
}

void AISCharacterBase::AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	
}

void AISCharacterBase::AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	
}

void AISCharacterBase::MulticastHandleDeath_Implementation()
{
	
}

void AISCharacterBase::Die()
{
	
}

AISCharacterBase* AISCharacterBase::GetAvaActor_Implementation()
{
	return this;  //接口获取自身
}
