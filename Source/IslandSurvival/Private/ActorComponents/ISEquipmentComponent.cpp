// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISEquipmentComponent.h"

// Sets default values for this component's properties
UISEquipmentComponent::UISEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UISEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UISEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

