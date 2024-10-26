// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ISItemBase.h"
#include "ISEquipable.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISEquipable : public AISItemBase
{
	GENERATED_BODY()
public:
	USceneComponent*GetAttachTarget(APawn*TargetPawn) const;
	void SetEquipableCollision();
public:
	virtual void UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) const override;
	virtual void UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) const override;
};
