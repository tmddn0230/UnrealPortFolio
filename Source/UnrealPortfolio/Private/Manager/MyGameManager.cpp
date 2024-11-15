// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyGameManager.h"
// UE
#include "Kismet/Gameplaystatics.h"
// Common
#include "Common/MyConfig.h"


UMyGameManager::UMyGameManager()
{
}

void UMyGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	// InitializeDependency �� ����Ͽ� GameManager ���� ������ �ش� �����ν��Ͻ����� ����
	Collection.InitializeDependency(UMyConfig::StaticClass());
	// ����
	// ���̺�
	// Http �� ���� ���� �������� �ϴ� �͵�

	Super::Initialize(Collection);

	// SaveData ��� ���� �� �ε�(�����)

	// ���Ϸ� ����� local profile ������ �о� ���δ�. 

	// Network Session Task , OnlineSystem ����

	// TrainingManager �� ����(Uobject) , NetworkMangager(GameInstanceSubSystem)�� ����

}

void UMyGameManager::Deinitialize()
{
    // �ܼ� ��� ����
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
	return false;
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
	//if (!IsInitialzied()) {
	//	UE_LOG(LogRs, Log, TEXT("GameManager is not Initialized"));
	//	return;
	//}
	bSinglePlay = false;
	Play_Type = EUP_PlayType::E_Server;
	UGameplayStatics::OpenLevel(this, FName(*UMyConfig::Lobby_MapName));
	// Session ����

	// Listen Server Play
	bListenServer = true;  //`` listen server bool : set
	GetWorld()->ServerTravel(UMyConfig::Game_MapName + TEXT("?listen"));
}

void UMyGameManager::Start_Client(const FString& InDeviceName, const FString& InDeviceId)
{

}

void UMyGameManager::Start_Control()
{
}

void UMyGameManager::Start_Observer()
{
}

void UMyGameManager::Start_Replay(const FString& TrainingID, const FString& UserID, const FString& ScenarioName)
{
}
