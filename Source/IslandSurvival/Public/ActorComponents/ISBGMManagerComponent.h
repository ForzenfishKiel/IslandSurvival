// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/StreamableManager.h"
#include "ISBGMManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EBGMType : uint8
{
	MainMenu,
	LevelDefault,
	BossBattle,
	Victory,
	Defeat
};
//角色的BGM播放管理器
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISBGMManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UISBGMManagerComponent();
	
	// 主接口
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayBGM(EBGMType Type, float FadeTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopBGM(float FadeTime = 1.0f);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	// 音频资源映射
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EBGMType, TSoftObjectPtr<USoundBase>> BGMLibrary;
	// 异步加载句柄
	TSharedPtr<FStreamableHandle> AssetLoaderHandle;

	// 当前播放组件 该组件不会被保存和加载
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> CurrentAudioComponent;

	// 加载并播放逻辑
	void LoadAndPlayBGM(EBGMType Type, float FadeTime);
	void OnAssetLoaded(EBGMType Type, float FadeTime);

	//切换音乐
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SwitchingBGM();

	// 淡出控制
	void FadeOutCurrent(float FadeTime);
	FTimerHandle FadeTimerHandle;

	UPROPERTY()
	EBGMType ISCurrentBgmType;
		
};
