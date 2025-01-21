// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ISWaitActorOwnerSet.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorOwnerSetChanged,bool,Result);

UCLASS(meta=(HideThen=true))
class ISLANDSURVIVAL_API UISWaitActorOwnerSet : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UISWaitActorOwnerSet* WaitActorOwnerSet(APlayerController* InController,UISItemsContainer* TargetContainer);

	UPROPERTY(BlueprintAssignable)
	FOnActorOwnerSetChanged OnActorOwnerSetChanged;
	
protected:
	virtual void Activate() override;
private:
	UPROPERTY()
	APlayerController* SourcePC;

	UPROPERTY()
	UISItemsContainer* SourceContainer;
};
