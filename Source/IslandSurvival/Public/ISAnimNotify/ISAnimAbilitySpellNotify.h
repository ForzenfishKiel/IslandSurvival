// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ISAnimAbilitySpellNotify.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISAnimAbilitySpellNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventTag")
	FGameplayTag EventTagToSend;
};
