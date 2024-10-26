// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/ISChooseInventoryInputData.h"
#include "ISEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass,typename PressedFuncType>
	void BindChooseItemActions(UISChooseInventoryInputData*InputData,UserClass*UserObject,PressedFuncType PressedFunc);
};
template <class UserClass, typename PressedFuncType>
void UISEnhancedInputComponent::BindChooseItemActions(UISChooseInventoryInputData* InputData, UserClass* UserObject, PressedFuncType PressedFunc)
{
	check(InputData);
	//以此为每个数组成员进行函数回调的绑定
	for(FChooseInventory&ChooseInventory:InputData->ChooseInventory)
	{
		if(ChooseInventory.InputAction)
		{
			if(PressedFunc)  //判断回调函数可以进行
			{
				BindAction(ChooseInventory.InputAction,ETriggerEvent::Started,UserObject,PressedFunc,ChooseInventory.ChooseIndex);
			}
		}
	}
}
