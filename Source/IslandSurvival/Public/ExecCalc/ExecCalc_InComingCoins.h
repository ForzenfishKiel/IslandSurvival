// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_InComingCoins.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UExecCalc_InComingCoins : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_InComingCoins();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
