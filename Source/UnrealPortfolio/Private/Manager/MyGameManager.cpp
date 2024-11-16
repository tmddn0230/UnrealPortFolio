// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyGameManager.h"
// UE
#include "Kismet/Gameplaystatics.h"
// Common
#include "Common/MyConfig.h"
#include "Common/MyRpcCommon.h"

DEFINE_LOG_CATEGORY(LogPlayerInfo);
DEFINE_LOG_CATEGORY(LogGameManager);

UMyGameManager::UMyGameManager()
{
}

void UMyGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	// InitializeDependency 을 사용하여 GameManager 보다 빠르게 해당 게임인스턴스들을 실행
	Collection.InitializeDependency(UMyConfig::StaticClass());
	// 세션
	// 테이블
	// Http 등 값이 먼저 정해져야 하는 것들

	Super::Initialize(Collection);
	bInitialized = true;
	// SaveData 경로 지정 및 로드(재시작)

	// 파일로 저장된 local profile 정보를 읽어 들인다. 

	// Network Session Task , OnlineSystem 사용시

	// TrainingManager 를 실행(Uobject) , NetworkMangager(GameInstanceSubSystem)를 통해

}

void UMyGameManager::Deinitialize()
{
    // 콘솔 명령 해제
	for (IConsoleObject* AuditCmd : AuditCmds)
	{
		IConsoleManager::Get().UnregisterConsoleObject(AuditCmd);
	}
	AuditCmds.Empty();

	Super::Deinitialize();
}

void UMyGameManager::Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool bGamemode)
{
	if (bGamemode) {

		if (Current_GameMode == EUP_GameMode::E_None) {
			if (MapName.Contains(UMyConfig::Single_MapName)) {

				Current_GameMode = EUP_GameMode::E_SinglePlay;
			}
			else {
				Current_GameMode = EUP_GameMode::E_MultiPlay;
			}
		}
	}
	else {
        if (MapName.Contains(UMyConfig::Lobby_MapName)) {

			Current_GameMode = EUP_GameMode::E_Lobby;
		}
		else {
			Current_GameMode = EUP_GameMode::E_None;
		}
	}
}

bool UMyGameManager::IsInitialzied()
{
	if (!bInitialized)
		return false;

	// Mode 관련 코드 추후 작성
	return true;
}

void UMyGameManager::Quit_Program()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMyGameManager::Start_PlayMode()
{
	if (bStartModeRun)
		return;

	switch (Get_PlayType()) {

	case EUP_PlayType::E_Single:
	{
		UE_LOG(LogGameManager, Log, TEXT("E_Single"));
		Start_SingleMode();
	}
	break;
	case EUP_PlayType::E_Server:
	{
		UE_LOG(LogGameManager, Log, TEXT("E_Server"));
		Start_Server();
	}
	break;
	case EUP_PlayType::E_Client:
	{
		UE_LOG(LogGameManager, Log, TEXT("E_Trainee"));
		FString device_name;
		FString device_id;
		bool ck_name = FParse::Value(FCommandLine::Get(), TEXT("DeviceName="), device_name);
		bool ck_id = FParse::Value(FCommandLine::Get(), TEXT("DeviceId="), device_id);
		Start_Client(device_name, device_id);
	}
	break;
	case EUP_PlayType::E_Control:
	{
		UE_LOG(LogGameManager, Log, TEXT("E_Control"));
		Start_Control();
	}
	break;
	case EUP_PlayType::E_Observer:
	{
		UE_LOG(LogGameManager, Log, TEXT("E_Observer"));
		FString device_name;
		bool ck_name = FParse::Value(FCommandLine::Get(), TEXT("DeviceName="), device_name);
		Start_Observer(device_name);
	}
	break;
	case EUP_PlayType::E_Replay:
	{
		UE_LOG(LogGameManager, Log, TEXT("E_Replay"));
		FString replay_TID;
		FString replay_UID;
		FString replay_SCN;

		// Get CommandLine Data to search on Web Server for Replay 
		/*
		*  CommandLine Info : 훈련ID, 훈련생 ID, 시나리오 이름
		*/
		FString commandLine = FCommandLine::Get();
		TArray<FString> result;
		commandLine.ParseIntoArray(result, TEXT(":"), true);

		TArray<FString> result2;
		result[1].ParseIntoArray(result2, TEXT("/"), true);

		replay_TID = result2[0];
		replay_UID = result2[1];
		replay_SCN = result2[2];

		Start_Replay(replay_TID, replay_UID, replay_SCN);
	}
	break;

	default:
		bStartModeRun = true;
		UE_LOG(LogGameManager, Log, TEXT("E_None"));
		break;
	}
}

// CES
void UMyGameManager::Start_SingleMode()
{
	// Start Single Play 

	// Start mode function Run
	bStartModeRun = true;
	bSinglePlay = true;
	Play_Type = EUP_PlayType::E_Single;
	UGameplayStatics::OpenLevel(this, FName(*UMyConfig::Single_MapName));
}

// Listen Server
void UMyGameManager::Start_Server()
{
    if (!IsInitialzied()) 
    	return;
	bSinglePlay = false;
	Play_Type = EUP_PlayType::E_Server;
	UGameplayStatics::OpenLevel(this, FName(*UMyConfig::Lobby_MapName));
	// Session 생략

	// Listen Server Play
	bListenServer = true;  //`` listen server bool : set
	GetWorld()->ServerTravel(UMyConfig::Game_MapName + TEXT("?listen"));
}

void UMyGameManager::Start_Client(const FString& InDeviceName, const FString& InDeviceId)
{
    if (!IsInitialzied())
    	return;

	Play_Type = EUP_PlayType::E_Client;
	FString options = FString::Printf(TEXT("%s=%d?%s=%s?%s=%s"),
		*MyString_Option::PlayerType, Play_Type,
		*MyString_Option::DeviceName, *InDeviceName,
		*MyString_Option::DeviceId, *InDeviceId
	);
	if (auto* world = GetWorld()) {
		bStartModeRun = true;
		UGameplayStatics::OpenLevel(GetWorld(), UMyConfig::Get_ServerLevel(), true, options);
		// options 를 통해 커맨드라인을 보냄
	}
}

void UMyGameManager::Start_Control()
{
	// Map Off
	if (auto* world = GetWorld()) {
		UGameplayStatics::SetEnableWorldRendering(world, false);
	}
    if (!IsInitialzied())
    	return;

	Play_Type = EUP_PlayType::E_Control;
	//InPc->Set_PlayType(Play_Type);
	FString options = FString::Printf(TEXT("%s=%d?%s=%s?%s=%s"),
		*MyString_Option::PlayerType, Play_Type,
		*MyString_Option::DeviceName, L"ControlPC",
		*MyString_Option::DeviceId,   L"001"
	);
	bStartModeRun = true;
	UGameplayStatics::OpenLevel(GetWorld(), UMyConfig::Get_ServerLevel(), true, options);
}

void UMyGameManager::Start_Observer(const FString& InDeviceName)
{
	if (!IsInitialzied())
	   return;

	Play_Type = EUP_PlayType::E_Observer;
	// Just Init Options , * we need code for fill this options
	FString options = FString::Printf(TEXT("%s=%d?%s=%s?%s=%s?%s=%s?%s=%s?%s=%s"),
		*MyString_Option::PlayerType, Play_Type,
		*MyString_Option::DeviceName, *InDeviceName,
		*MyString_Option::DeviceId, L"DeviceID"
	);

	bStartModeRun = true;
	UGameplayStatics::OpenLevel(GetWorld(), UMyConfig::Get_ServerLevel(), true, options);
}

void UMyGameManager::Start_Replay(const FString& TrainingID, const FString& UserID, const FString& ScenarioName)
{
	if (!IsInitialzied())
	   return;

	Play_Type = EUP_PlayType::E_Replay;
	// Just Init Options , * we need code for fill this options
	FString options = FString::Printf(TEXT("%s=%d"),
		*MyString_Option::PlayerType, Play_Type
	);
	bStartModeRun = true;
	// Maybe TrainingMap
	UGameplayStatics::OpenLevel(GetWorld(), FName(*UMyConfig::Replay_MapName), true, options);

	// ReplayManager
	//if (auto* replay_mgr = gi->GetSubsystem<URtReplayManager>()) {
	//	// Set ReplayFileName
	//	replay_mgr->ReplayInfo.SelectedTID = FCString::Atoi(*TID);
	//	replay_mgr->ReplayInfo.SelectedUID = FCString::Atoi(*UID);
	//	replay_mgr->ReplayInfo.SelectedScenarioName = SCN;
	//}
}
