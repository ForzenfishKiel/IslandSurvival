// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ISItemBase.h"
#include "ISEquipable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterEquipState:uint8
{
	None = 0 UMETA(DisplayName = "None"),
	EquipAxe = 1 UMETA(DisplayName = "EquipAxe"),
	EquipSword = 2 UMETA(DisplayName = "EquipSword"),
	EquipBow = 3 UMETA(DisplayName = "EquipBow"),
};
UCLASS()
class ISLANDSURVIVAL_API AISEquipable : public AISItemBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EquipState")
	ECharacterEquipState CurrentEquipState = ECharacterEquipState::None;
	USceneComponent*GetAttachTarget(APawn*TargetPawn) const;
	void SetEquipableCollision();
public:
	virtual void UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) const override;
	virtual void UnUseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) const override;
};
