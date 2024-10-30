// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "ISInputAbilityData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInputData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
UCLASS()
class ISLANDSURVIVAL_API UISInputAbilityData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadwrite)
	TArray<FInputData> InputData;
};
