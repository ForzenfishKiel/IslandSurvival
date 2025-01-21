// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ISCharacterBase.h"

AISCharacterBase::AISCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AISCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
void AISCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AISCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AISCharacterBase::ActivateCharacterAbility_Implementation()
{
	IISCombatInterface::ActivateCharacterAbility_Implementation();
}

void AISCharacterBase::EndActivateCharacterAbility_Implementation()
{
	IISCombatInterface::EndActivateCharacterAbility_Implementation();
}

void AISCharacterBase::InitializePlayerAttribute(UAbilitySystemComponent* ASC,
	TSubclassOf<UGameplayEffect> AttributeClass)
{
	
}

void AISCharacterBase::AddCharacterActivateAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	
}

void AISCharacterBase::AddCharacterPassiveAbility(TArray<TSubclassOf<UGameplayAbility>>& TargetActivateAbilities)
{
	
}

void AISCharacterBase::InitAbilityActorInfo()
{
	
}

void AISCharacterBase::MulticastHandleDeath_Implementation()
{
	
}

void AISCharacterBase::Die()
{
	
}

void AISCharacterBase::BindAttributeSet() const
{
	
}

AISCharacterBase* AISCharacterBase::GetAvaActor_Implementation()
{
	return this;  //接口获取自身
}

FName AISCharacterBase::GetCharacterName_Implementation()
{
	return CharacterName;
}

int32 AISCharacterBase::GetLevel_Implementation()
{
	return 1;
}

FTaggedMontage AISCharacterBase::FindMontageFromTag_Implementation(const FGameplayTag Tag) const
{
	if(!Tag.IsValid()) return FTaggedMontage();
	for(const FTaggedMontage MontageTag : CharacterTaggedMontage)
	{
		if(MontageTag.AnimMontageTag.MatchesTagExact(Tag))
		{
			return MontageTag;
		}
	}
	return FTaggedMontage();
}
