// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ISItemBase.h"
#include "ISConsumable.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISConsumable : public AISItemBase
{
	GENERATED_BODY()
public:
	virtual void UseItem(AActor* TargetCharacter, UAbilitySystemComponent* TargetASC) override;
	virtual void ApplyEffectToTarget(UAbilitySystemComponent* InASC, TSubclassOf<UGameplayEffect> EffectClass) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Config")
	TSubclassOf<UGameplayEffect> ConsumableEffectClass;
};
