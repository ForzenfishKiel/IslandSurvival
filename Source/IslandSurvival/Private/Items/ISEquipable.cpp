// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ISEquipable.h"

#include "Character/ISCharacter.h"

USceneComponent* AISEquipable::GetAttachTarget(APawn* TargetPawn) const
{
	USceneComponent*AttachTarget = TargetPawn->GetRootComponent();
	if(AISCharacter*TargetCharacter = Cast<AISCharacter>(TargetPawn))
	{
		AttachTarget = TargetCharacter->GetMesh();
	}
	return AttachTarget;
}

void AISEquipable::SetEquipableCollision()
{
	PickUpCheckSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
}

void AISEquipable::UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) const
{
	Super::UseItem(TargetCharacter, TargetASC);
}

void AISEquipable::UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) const
{
	Super::UnUseItem(TargetCharacter, TargetASC);
}
