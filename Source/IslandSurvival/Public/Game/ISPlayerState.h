// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ISPlayerState.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class ISLANDSURVIVAL_API AISPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AISPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet*GetAttributeSet() const{return ISAttributeSet;}
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ISAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet>ISAttributeSet;
};
