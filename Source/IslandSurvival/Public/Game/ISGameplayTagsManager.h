// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FGameplayTagsManager
{
	public:
		static const FGameplayTagsManager&Get() {return GameplayTags;}
		static void InitializeNativeGameplayTags();
	private:
		static FGameplayTagsManager GameplayTags;
};