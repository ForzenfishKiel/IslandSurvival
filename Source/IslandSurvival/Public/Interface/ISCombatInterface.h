// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "ISCombatInterface.generated.h"

class FOnCharacterAbilityEnd;
class AISCharacterBase;
// This class does not need to be modified.
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AnimMontageTag;  //触发动画的蒙太奇

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FName> CombatTipSocketName;  //触发技能的骨骼组
};
UINTERFACE(MinimalAPI)
class UISCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISLANDSURVIVAL_API IISCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	void ActivateCharacterAbility();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	void EndActivateCharacterAbility();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	AISCharacterBase* GetAvaActor();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	FName GetCharacterName();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	int32 GetLevel();
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage FindMontageFromTag( const FGameplayTag Tag ) const;
	UFUNCTION( BlueprintNativeEvent,BlueprintCallable)
	FGameplayAbilitySpecHandle FindActivateAbility(const FGameplayTag InTag) const;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ApplyDamageToTarget(AActor* Target);
	virtual void Die() = 0;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	USkeletalMeshComponent* GetTargetSkeletalMeshComponent();
};
