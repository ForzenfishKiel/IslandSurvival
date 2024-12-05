// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "BuildingSystem/ActorComponents/ISStorageContainer.h"
#include "ISStorageMaster.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISStorageMaster : public AISBuildingSystemBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void OnBuildingWasInteract_Implementation(const AActor* InteractingActor, const UActorComponent* InteractingComponent) override;
public:
	AISStorageMaster();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UUserWidget> StorageUserWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UISMenuUIBase> StorageWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISStorageContainer>StorageContainer;

	void StorageUIOpen(APlayerController* TargetController);
	UFUNCTION()
	void StorageUIClose(APlayerController* TargetController);
};
