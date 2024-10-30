// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ISInputAbility.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISInputAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FGameplayTag InputTag;
};
