// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/ISCharacterBase.h"
#include "ISEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISEnemy : public AISCharacterBase,public IAbilitySystemInterface
{
	GENERATED_BODY()
	AISEnemy();
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> ISEnemyAbilitysystem;
	UPROPERTY()
	TObjectPtr<UAttributeSet> ISEnemyAttribute;

public:
	virtual void Die() override;
	virtual void MulticastHandleDeath() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetEnemyAttribute() const;
};
