// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "BuildingSystem/ActorComponents/ISStroageContainer.h"
#include "UI/ISMenuUIBase.h"
#include "ISStorageMaster.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISStorageMaster : public AISBuildingSystemBase
{
	GENERATED_BODY()
public:
	AISStorageMaster();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UISStroageContainer>StorageContainer;

	UPROPERTY()
	TObjectPtr<UISMenuUIBase> StorageWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget>StorageUserWidgetClass;

	UFUNCTION(Client,Reliable)
	void StorageUIOpen(APlayerController* TargetController);
	UFUNCTION(Client,Reliable)
	void StorageUIClose(APlayerController* TargetController);
	virtual void InteractOnServer(AController* InPlayerController) override;
	virtual void OnBuildingWasInteract_Implementation(const AActor* InteractingActor, const UActorComponent* InteractingComponent) override;
	virtual void InitItemConfig() override;
};
