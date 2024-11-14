// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitCharacterSpeedChangeToZero.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpeedChange);
/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UWaitCharacterSpeedChangeToZero : public UAbilityTask
{
	GENERATED_BODY()
	UPROPERTY(BlueprintAssignable)
	FOnSpeedChange OnSpeedChange;

	UFUNCTION(BlueprintCallable,Category="Ability|Tasks",meta = (DisplayName = "WaitCharacterSpeedChangeToZero",HidePin = "OwingAbility"
		,DefaultToSelf = "OwingAbility",BlueprintInternalUseOnly = "true"))
	static UWaitCharacterSpeedChangeToZero* DetectSpeedChange(UGameplayAbility*OwingAbility);

protected:
	virtual void TickTask(float DeltaTime) override;
};
