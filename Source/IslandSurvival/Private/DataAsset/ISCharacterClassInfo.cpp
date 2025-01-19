// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ISCharacterClassInfo.h"

FCharacterClassDefaultInfo UISCharacterClassInfo::GetCharacterClassData(const FName InTargetName)
{
	return CharacterClassInfomation.FindChecked(InTargetName);
}
