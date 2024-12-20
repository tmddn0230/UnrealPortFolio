// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Common
#include "Common/MyRpcCommon.h"
#include "MyConfig.generated.h"

UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogMyConfig, Log, All);

UCLASS()
class UNREALPORTFOLIO_API UMyConfig : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static FString MySettingsFile;

	static FString Single_MapName;     // Single
	static FString Main_MapName;     // Server 
	static FString Lobby_MapName;    // AsyncLoading Map
	static FString Game_MapName;
	static FString Replay_MapName;
	static FString Device_Name;
	static int32   Device_Index;
	
	// Url
	static FString ListenIP;
	static FString HttpBaseURL;

	// Socket
	static FString TCPSocketIP;
	static FString UDPSocketIP;
	static int32 TCPPort;
	static int32 UDPPort;

	// Voice chat
	static bool UsingVoice;

	// for 10 or 20 ver
	static int32 PlayerNum;

	// stringtable
	static FString MessageBoxPath;
	static FString ControlMainPath;
	static FString ErrorMsgPath;
	static FString ResultDetailPath;

	// nation
	static int32 NationCode;

	// PlayInitSetting
	static FPlayInitSetting PlayInitSettings;

// Functions
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Load
	static bool LoadXMLFile();
	static FPlayInitSetting Setting_From_xmlFile(const FString& TempTag, const FString& TempString);
	static void Setting_From_InitFile(const FString& InIni, bool InUseDefaultVaule);
	static void LoadINIFile();

	// Get
	static const FName& Get_ServerLevel();
	static const int32 Get_NationCode();
};
