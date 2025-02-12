// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/ISBuildingSystemBase.h"
#include "Components/ActorComponent.h"
#include "Game/ISPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ISInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UISInteractionComponent();
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void PrimaryInteract();
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void SecondaryInteract();
	void TickInteractline();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	TObjectPtr<AActor>LastActor = nullptr;

	UPROPERTY(Replicated,BlueprintReadOnly)
	TObjectPtr<AActor> InteractedActor;
	UFUNCTION(Server,Reliable)
	void ClearInteractedActor();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	TObjectPtr<UActorComponent> LastComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AISBuildingSystemBase>LastBuildingActor = nullptr;

	UPROPERTY()
	TObjectPtr<AISBuildingSystemBase> CurrentBuilding = nullptr;

	UPROPERTY()
	TObjectPtr<AISCharacter> SourceCharacter;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsInteractTrace = false;

	template<typename T>
	UFUNCTION(Server,Reliable)
	void PossessedObjectOnServer(T* Building);
	UFUNCTION()
	void ReciveControllerOpenUIEvent(APlayerController* InController);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};

