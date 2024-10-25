// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/ISItemInterface.h"
#include "ISItemBase.generated.h"

UCLASS()
class ISLANDSURVIVAL_API AISItemBase : public AActor,public IISItemInterface
{
	GENERATED_BODY()
	
public:	
	AISItemBase();
	UPROPERTY(EditAnywhere)
	FName ItemID;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent>PickUpCheckSphere;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ItemsStaticMesh;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PickUpItemToInventory_Implementation(APawn* TargetPawn, AActor* TargetActor) override;
};
