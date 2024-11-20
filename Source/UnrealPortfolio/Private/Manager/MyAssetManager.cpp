// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyAssetManager.h"
// Data
#include "Data/MyModeDefinition.h"

// Log
DEFINE_LOG_CATEGORY(LogMyAssetManager);

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

UMyAssetManager& UMyAssetManager::Get()
{
	UMyAssetManager* This = Cast<UMyAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		return *NewObject<UMyAssetManager>(); // never calls this
	}
}

void UMyAssetManager::Preload_ModeDefinition(FStreamableDelegate DelegateToCall)
{
	FPrimaryAssetType AssetType = FPrimaryAssetType(UMyModeDefinition::StaticClass()->GetFName());
	TArray< FPrimaryAssetId> AssetsToLoad;
	GetPrimaryAssetIdList(AssetType, AssetsToLoad);
	TArray<FName> LoadBundles;

	LoadHandle = LoadPrimaryAssets(AssetsToLoad, LoadBundles);

	if (LoadHandle.IsValid())
	{
		if (!LoadHandle->HasLoadCompleted())
		{
			LoadHandle->BindCompleteDelegate(DelegateToCall);
			return;
		}
	}
}

UMyModeDefinition* UMyAssetManager::ForceLoad_ModeDefinition(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ObjPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UMyModeDefinition* obj = Cast<UMyModeDefinition>(ObjPath.TryLoad());

	if (bLogWarning && obj == nullptr)
	{
		UE_LOG(LogMyAssetManager, Warning, TEXT("Failed to obj for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return obj;
}

UMyModeDefinition* UMyAssetManager::Load_ModeDefinition()
{
	FPrimaryAssetId AssetId = FPrimaryAssetId(FPrimaryAssetType(UMyModeDefinition::StaticClass()->GetFName()), "ModeDefinition");
	return ForceLoad_ModeDefinition(AssetId, true);
}

UObject* UMyAssetManager::Get_Asset(const FName InAssetName, bool bLogWarning)
{
	FPrimaryAssetId AssetId = FPrimaryAssetId(FPrimaryAssetType(InAssetName), InAssetName);
	FSoftObjectPath ObjPath = GetPrimaryAssetPath(AssetId);
	// This does a synchronous load and may hitch
	UObject* obj = ObjPath.TryLoad();
	if (bLogWarning && obj == nullptr) {
		UE_LOG(LogMyAssetManager, Warning, TEXT("Failed to obj for identifier %s!"), *AssetId.ToString());
	}
	return obj;
}
