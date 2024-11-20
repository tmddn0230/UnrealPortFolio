// Fill out your copyright notice in the Description page of Project Settings.
// 게임 어셋 관리자.
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MyAssetManager.generated.h"

class UMyModeDefinition;

// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogMyAssetManager, Log, All);

/**
 * UPrimaryDataAsset 비동기 로드 매니저 : 캐릭터 클래스, 무기 타입 등등 
 */
UCLASS()
class UNREALPORTFOLIO_API UMyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	// Complete Load Handle
	TSharedPtr<FStreamableHandle> LoadHandle;

public:
	virtual void StartInitialLoading() override;

	/** Returns the current AssetManager object */
	static UMyAssetManager& Get();

	// Mode 
	void Preload_ModeDefinition(FStreamableDelegate DelegateToCall);
	UMyModeDefinition* ForceLoad_ModeDefinition(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning);
	UMyModeDefinition* Load_ModeDefinition();


	UObject* Get_Asset(const FName InAssetName, bool bLogWarning = true);
};
