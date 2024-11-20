# UnrealPortFolio
언리얼엔진 5.1 프로젝트 포트폴리오 입니다.



# <u>데이터 관련</u>

플레이 타입에 따른 게임 에셋 로드 

UPrimaryDataAsset 을 상속 받은 클래스에 플레이 모드선언

UAssetManager 를 상속 받은 MyAssetManager 를 통해 PrimaryAssetType 의 게임 에셋을 비동기 로드 한다.

:computer: 코드 예시

<details>
<summary>MyPrimayDataAsset</summary>
	
```cpp
	UCLASS()
class UNREALPORTFOLIO_API UMyModeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FMyModeInfo SingleMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ControlMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ClientMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ObserveMode;
	
	UPROPERTY(EditAnywhere)
	FMyModeInfo ServerMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ReplayMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo DefaultMode;
public:
	FName Get_UI(EUP_PlayType InPlayType);
	const FMyModeInfo& Get_Mode(EUP_PlayType InPlayType);
	
};
```
</details>

<details>
<summary>MAssetManager</summary>

```cpp
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
```


</details>

