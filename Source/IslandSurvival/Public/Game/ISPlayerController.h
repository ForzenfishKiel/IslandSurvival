// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "DataAsset/ISChooseInventoryInputData.h"
#include "ISPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API AISPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AISPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UISChooseInventoryInputData>ChooseHotBarInputData;
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext>CharacterInputMapping;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext>CharacterInputMenuMapping;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>IA_Move;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>IA_LookUp;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>IA_Interact;  //交互判定
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>IA_OpenUI;
	void Move(const struct FInputActionValue&InputActionValue);
	void LookUp(const struct FInputActionValue&InputActionValue);
	void PrimaryInteract();
	void ChooseHotBar(int32 InputIndex);
	UFUNCTION(Client,Reliable)
	void OpenUI(); //打开UI应该在客户端完成，因为其他客户端不需要被打开
};
