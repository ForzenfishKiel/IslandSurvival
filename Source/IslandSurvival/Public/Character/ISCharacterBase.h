// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interface/ISCombatInterface.h"
#include "ISCharacterBase.generated.h"

//初始化结构体
USTRUCT()
struct FCharacterParams
{
	GENERATED_BODY()
	FCharacterParams(){}
	FCharacterParams(UAbilitySystemComponent*ASC,UAttributeSet*AS,APlayerController*PC,APlayerState*PS):SourceASC(ASC),SourceAS(AS),SourcePC(PC),SourcePS(PS){}
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> SourceAS = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerController>SourcePC = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerState> SourcePS = nullptr;
};
UCLASS()
class ISLANDSURVIVAL_API AISCharacterBase : public ACharacter,public IISCombatInterface
{
	GENERATED_BODY()

public:
	AISCharacterBase();

	UPROPERTY(EditDefaultsOnly,Category = "Config")
	FName CharacterName; //此角色的身份
protected:
	virtual void BeginPlay() override;

public:
	//自身虚函数
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ActivateCharacterAbility_Implementation() override;
	virtual void EndActivateCharacterAbility_Implementation() override;
	virtual void InitializePlayerAttribute(UAbilitySystemComponent* ASC,TSubclassOf<UGameplayEffect>AttributeClass);
	virtual void AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>&TargetActivateAbilities);
	virtual void AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>&TargetActivateAbilities);
	virtual void InitAbilityActorInfo();
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();
	virtual void Die();  //角色死亡
	virtual void BindAttributeSet() const;
	//ICombatInterface虚函数
	virtual AISCharacterBase* GetAvaActor_Implementation() override;
	virtual FName GetCharacterName_Implementation() override;
	virtual int32 GetLevel_Implementation() override;
	virtual FTaggedMontage FindMontageFromTag_Implementation(const FGameplayTag Tag) const override;
private:
	UPROPERTY(EditDefaultsOnly,Category = "Config")
	TArray<FTaggedMontage> CharacterTaggedMontage;
};



