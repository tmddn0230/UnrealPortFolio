# UnrealPortFolio
언리얼엔진 5.1 프로젝트 포트폴리오 입니다.

## <u>Unreal Engine Listen Server Framework</u>

:computer: 코드 예시

GameMode
<details>
<summary>MyGameMode</summary>
	
```cpp

```
</details>

<details>
<summary>MyGameState</summary>
	
```cpp

```
</details>

<details>
<summary>MyPlayerController</summary>
	
```cpp

```
</details>

<details>
<summary>MyPlayerState</summary>
	
```cpp

```
</details>

## <u>UI Widget</u>

:computer: 코드 예시

Primary Widget 
<details>
<summary>MyUI_PrimayLayout</summary>
	
```cpp

```
</details>

Page Widget 
<details>
<summary>MyUI_PageBase</summary>
	
```cpp

```
</details>

HUD
<details>
<summary>MyGameHUD</summary>
	
```cpp

```
</details>



## <u>File I/O</u>

:computer: 코드 예시

XML 파일 읽기
<details>
<summary>MyConfig</summary>
	
```cpp

```
</details>

JSON 파일 읽기 & 쓰기 
<details>
<summary>MyJsonLibFunction</summary>
	
```cpp

```
</details>

## <u>음성채팅 및 녹음, 리플레이</u>

:computer: 코드 예시

Voice Chat with UDP
<details>
<summary>MyVoiceManager</summary>
	
```cpp

```
</details>

Voice Recording 
<details>
<summary>MyVoiceManager</summary>
	
```cpp

```
</details>

Voice Mixing 
<details>
<summary>MyReplayManager</summary>
	
```cpp

```
</details>

## <u>데이터 관련</u>

DataTable 사용

:computer: 코드 예시

<details>
<summary>MyTableRows</summary>
	
```cpp

```
</details>

<details>
<summary>MyTableManager</summary>
	
```cpp

```
</details>



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


## <u>네트워크 관련</u>

HttpRequest 클라이언트 모델
- 로그인 
- 파일 업, 다운로드

:computer: 코드 예시

<details>
<summary>MyHttpManaer</summary>

URL, Verb , Header 와 같이 반복되는 작업

```cpp
TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyHttpManager::SetRequestOptions(const FString& URL)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	return Request;
}
```

Request에 적용할 Json String 을 만드는 코드도 많이 중복되어 나오기 때문에 따로 작성

```cpp

FString UMyHttpManager::GetContentString(const TSharedRef<FJsonObject>& requestObj )
{
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(requestObj, Writer);

	return RequestBody;
}
```

전체 로그인 코드 

```cpp

void UMyHttpManager::Login(const FString& InID, const FString& InPassword)
{
	// Main
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(MainURL);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &UMyHttpManager::OnResponse_Login);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("protocol", FString::FromInt((int32)EWebProtocolType::webLogin));
	RequestObj->SetStringField("account", InID);
	RequestObj->SetStringField("pass", InPassword);

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogMyHttpManager, Warning, TEXT("Login Request Called"));
}

void UMyHttpManager::OnResponse_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response == nullptr) return;
	if (!bWasSuccessful) return;

	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// Try to convert string to json object. Output goes in RetJsonObject
	if (FJsonSerializer::Deserialize(jsonReader, JsonObject))
	{
		int32 protocol;
		if (JsonObject->TryGetNumberField(TEXT("protocol"), protocol))
		{
			UE_LOG(LogMyHttpManager, Log, TEXT("Protocol : %d"), protocol);
		}

		TSharedPtr<FJsonObject> ResultObject;
		FString Result = JsonObject->GetStringField("result");
		TSharedRef<TJsonReader<>> resultReader = TJsonReaderFactory<>::Create(Result);

		if (FJsonSerializer::Deserialize(resultReader, ResultObject))
		{
			// Get Login Result
			int32 ResultValue = ResultObject->GetIntegerField(TEXT("RESULT"));
			// Process Login On Widget
			Handle_LoginCheck.Broadcast(ResultValue);
		}
	}
}
```

</details>



