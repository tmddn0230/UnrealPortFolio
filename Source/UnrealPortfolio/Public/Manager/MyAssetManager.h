// Fill out your copyright notice in the Description page of Project Settings.
// ���� ��� ������.
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MyAssetManager.generated.h"

class UMyModeDefinition;

// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogMyAssetManager, Log, All);

/**
 * UPrimaryDataAsset �񵿱� �ε� �Ŵ��� : ĳ���� Ŭ����, ���� Ÿ�� ��� 
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
