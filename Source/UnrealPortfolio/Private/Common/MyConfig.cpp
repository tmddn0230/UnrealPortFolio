// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MyConfig.h"
#include "XmlParser.h"

DEFINE_LOG_CATEGORY(LogMyConfig);

// ini 파일 위치
FString UMyConfig::MySettingsFile = TEXT("My/MySettings.ini");

FString UMyConfig::Solo_MapName;
FString UMyConfig::Main_MapName;
FString UMyConfig::Lobby_MapName;
FString UMyConfig::Game_MapName;
FString UMyConfig::Replay_MapName;
FString UMyConfig::Device_Name;
int32   UMyConfig::Device_Index;

// Url
FString UMyConfig::ListenUrl;
FString UMyConfig::WebServerUrl;
FString UMyConfig::WebScenarioUrl;
FString UMyConfig::WebReplayUploadUrl;
FString UMyConfig::WebReplayDownloadUrl;
FString UMyConfig::WebLogUrl;

// Socket
FString UMyConfig::TCPSocketIP;
FString UMyConfig::UDPSocketIP;
int32   UMyConfig::TCPPort;
int32   UMyConfig::UDPPort;

// Voice Chat
bool    UMyConfig::UsingVoice;

// for 10 or 20 ver
int32   UMyConfig::PlayerNum;

// stringtable
FString UMyConfig::MessageBoxPath;
FString UMyConfig::ControlMainPath;
FString UMyConfig::ErrorMsgPath;
FString UMyConfig::ResultDetailPath;

// nation Cde
int32   UMyConfig::NationCode;

void UMyConfig::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadINIFile();    
	LoadXMLFile();    
}

void UMyConfig::Deinitialize()
{
	Super::Deinitialize();
}

bool UMyConfig::LoadXMLFile()
{
	FString file_path = TEXT("TrainingData/IpConfig/RtIpConfig.xml");
	FString directory;
	if (GIsEditor) {
		directory = FPaths::ProjectDir();
	}
	else {
		directory = FPaths::LaunchDir();
	}
	FString file_url = directory + file_path;
	FPaths::MakeStandardFilename(file_url);

	bool bResult;
	FXmlFile XmlFile;
	bResult = XmlFile.LoadFile(file_url, EConstructMethod::ConstructFromFile);

	if (!XmlFile.IsValid())
	{
		UE_LOG(LogMyConfig, Error, TEXT("Can't Read File :%s"), *file_url);
		return false;
	}


	const TArray<FXmlNode*>& Nodes = XmlFile.GetRootNode()->GetChildrenNodes();
	for (int i = 0; i < Nodes.Num(); i++)
	{
		const FString& TempTag = Nodes[i]->GetTag();
		const FString& TempString = Nodes[i]->GetContent();
		Setting_From_xmlFile(TempTag, TempString);
	}

	return true;
}

void UMyConfig::Setting_From_xmlFile(const FString& TempTag, const FString& TempString)
{
	if (TempTag == TEXT("ServerIp")) {
		ListenUrl = TempString;
	}
	else if (TempTag == TEXT("TCPSocketIp")) {
		TCPSocketIP = TempString;
	}
	else if (TempTag == TEXT("UDPSocketIp")) {
		UDPSocketIP = TempString;
	}
	else if (TempTag == TEXT("WebMainUrl")) {
		WebServerUrl = TempString;
	}
	else if (TempTag == TEXT("WebDownUrl")) {
		WebScenarioUrl = TempString;
	}
	else if (TempTag == TEXT("WebRepUpUrl")) {
		WebReplayUploadUrl = TempString;
	}
	else if (TempTag == TEXT("WebRepDnUrl")) {
		WebReplayDownloadUrl = TempString;
	}
	else if (TempTag == TEXT("WebLogUrl")) {
		WebLogUrl = TempString;
	}
	else if (TempTag == TEXT("NationCode")) {

		NationCode = FCString::Atoi(*TempString);
	}
}

void UMyConfig::LoadINIFile()
{
	//`` load game ini
	if (GIsEditor) {
		FString ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() / MySettingsFile);
		Setting_From_InitFile(ConfigFilePath, true);

	}
	else {
		FString ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() / MySettingsFile);
		Setting_From_InitFile(ConfigFilePath, true);
		ConfigFilePath = FPaths::ProjectConfigDir() / MySettingsFile;
		Setting_From_InitFile(ConfigFilePath, false);
	}
}

void UMyConfig::Setting_From_InitFile(const FString& InIni, bool InUseDefaultVaule)
{
	// From ini

	if (GConfig) {
		// Map Name Confirm
		GConfig->GetString(TEXT("MY_INIT"), TEXT("Solo_MapName"), Solo_MapName, InIni);
		GConfig->GetString(TEXT("MY_INIT"), TEXT("Main_MapName"), Main_MapName, InIni);
		GConfig->GetString(TEXT("MY_INIT"), TEXT("Lobby_MapName"), Lobby_MapName, InIni);
		GConfig->GetString(TEXT("MY_INIT"), TEXT("Game_MapName"),  Game_MapName, InIni);
		GConfig->GetString(TEXT("MY_INIT"), TEXT("Replay_MapName"), Replay_MapName, InIni);

		// Socket Port
		GConfig->GetInt(TEXT("MY_INIT"), TEXT("TCPPort"), TCPPort, InIni);
		GConfig->GetInt(TEXT("MY_INIT"), TEXT("UDPPort"), UDPPort, InIni);

		// Voice Chat
		GConfig->GetBool(TEXT("MY_INIT"), TEXT("Voice"), UsingVoice, InIni);

		// for Fix 10->20 
		GConfig->GetInt(TEXT("MY_INIT"), TEXT("PlayerNum"), PlayerNum, InIni);

	}
}


