// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UMMC_Attack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	UMMC_Attack();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:
	FGameplayEffectAttributeCaptureDefinition PlayerWeaponAttack;
};
