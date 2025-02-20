// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISBGMManagerComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UISBGMManagerComponent::UISBGMManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}




// Called when the game starts
void UISBGMManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UISBGMManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//开始播放bgm，异步加载资源以后，创建回调函数加载资产
void UISBGMManagerComponent::PlayBGM(EBGMType Type, float FadeTime)
{
	ISCurrentBgmType = Type;
	if(!BGMLibrary.Contains(Type)) return;
	
	if(!CurrentAudioComponent)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		if(ActiveAssetHandle)
		{
			ActiveAssetHandle->ReleaseHandle();
		}
		// 创建新句柄并存入容器
		ActiveAssetHandle = Streamable.RequestAsyncLoad(
			BGMLibrary[Type].ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &UISBGMManagerComponent::OnAssetLoaded, Type, FadeTime),
			FStreamableManager::AsyncLoadHighPriority,
			true // 不自动释放句柄
		);
	}
}

//停止播放bgm
void UISBGMManagerComponent::StopBGM(float FadeTime)
{
	FadeOutCurrent(FadeTime);
}

void UISBGMManagerComponent::LoadAndPlayBGM(EBGMType Type, float FadeTime)
{
	
}

//加载音频
void UISBGMManagerComponent::OnAssetLoaded(EBGMType Type, float FadeTime)
{
	if(USoundBase* LoadedSound = Cast<USoundBase>(BGMLibrary[Type].Get()))
	{
		// 淡出当前音乐
		FadeOutCurrent(FadeTime);

		// 创建新音频组件
		CurrentAudioComponent = UGameplayStatics::CreateSound2D(
			GetWorld(),
			LoadedSound,
			0.5f,
			1.0f,
			0.0f,
			nullptr,
			true,
			true
		);

		if(!CurrentAudioComponent) return;
		CurrentAudioComponent->SetIsReplicated(true);
		CurrentAudioComponent->OnAudioFinished.AddDynamic(this,&UISBGMManagerComponent::SwitchingBGM);
		// 设置淡入
		if(FadeTime > 0)
		{
			CurrentAudioComponent->FadeIn(FadeTime, 1.0f);
		}
		else
		{
			CurrentAudioComponent->Play();
		}
	}
}

//淡出并关闭当前的音频播放管理器
void UISBGMManagerComponent::FadeOutCurrent(float FadeTime)
{
	if(CurrentAudioComponent&&CurrentAudioComponent->IsPlaying())
	{
		if(FadeTime > 0)
		{
			CurrentAudioComponent->FadeOut(FadeTime, 0.0f);
			CurrentAudioComponent->DestroyComponent();
			UnLoadBGM(ISCurrentBgmType);
		}
		//没有淡出时间则直接停止
		else
		{
			CurrentAudioComponent->Stop();
			CurrentAudioComponent->DestroyComponent();
			UnLoadBGM(ISCurrentBgmType);
		}
	}
	else if(CurrentAudioComponent)
	{
		CurrentAudioComponent->Stop();
		UnLoadBGM(ISCurrentBgmType);
		CurrentAudioComponent->DestroyComponent();
	}
}

void UISBGMManagerComponent::SwitchingBGM()
{
	if(CurrentAudioComponent)
	{
		CurrentAudioComponent->DestroyComponent();  //删除当前播放管理器
		CurrentAudioComponent = nullptr;
		UnLoadBGM(ISCurrentBgmType);
		PlayBGM(ISCurrentBgmType,2.f);
	}
}

void UISBGMManagerComponent::UnLoadBGM(EBGMType Type)
{

	if(ActiveAssetHandle)
	{
		ActiveAssetHandle->ReleaseHandle();  //释放异步资源
	}
    
	// 强制卸载未被引用的资源
	UAssetManager::Get().UnloadPrimaryAssetsWithType(FPrimaryAssetType("BGM"));
}
