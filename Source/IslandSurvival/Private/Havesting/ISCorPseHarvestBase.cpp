// Fill out your copyright notice in the Description page of Project Settings.


#include "Havesting/ISCorPseHarvestBase.h"

#include "MovieSceneTracksComponentTypes.h"

AISCorPseHarvestBase::AISCorPseHarvestBase()
{
	HarvestStaticMesh->UnregisterComponent();  //取消注册静态网格体
	HarvestStaticMesh->DestroyComponent();

	CorPseRoot = CreateDefaultSubobject<USceneComponent>("CorPseRootComponent");
	CorPseRoot->SetupAttachment(GetRootComponent());

	CorPseSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CorPseSkeletalMesh");
	CorPseSkeletalMesh->SetupAttachment(CorPseRoot);
}
