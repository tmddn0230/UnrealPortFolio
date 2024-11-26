// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MyConfig.h"
#include "XmlParser.h"

DEFINE_LOG_CATEGORY(LogMyConfig);

// ini 파일 위치
FString UMyConfig::MySettingsFile = TEXT("My/MySettings.ini");

FString UMyConfig::Single_MapName;
FString UMyConfig::Main_MapName;
FString UMyConfig::Lobby_MapName;
FString UMyConfig::Game_MapName;
FString UMyConfig::Replay_MapName;
FString UMyConfig::Device_Name;
int32   UMyConfig::Device_Index;

// Url
FString UMyConfig::ListenIP;
FString UMyConfig::HttpBaseURL;

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

// Play Init Settings
FPlayInitSetting UMyConfig::PlayInitSettings;


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
	// 읽을 XML 파일의 위치
	FString file_path = TEXT("TrainingData/IpConfig/MyIpConfig.xml");
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
		// 각 노드의 값을 변수로
		PlayInitSettings = Setting_From_xmlFile(TempTag, TempString);
	}
	return true;
}

FPlayInitSetting UMyConfig::Setting_From_xmlFile(const FString& TempTag, const FString& TempString)
{
	FPlayInitSetting Infos;

	if (TempTag == TEXT("ServerIp")) {
		ListenIP = TempString;
		Infos.Set_ServerId(TempString);
	}
	else if (TempTag == TEXT("TCPSocketIp")) {
		TCPSocketIP = TempString;
	}
	else if (TempTag == TEXT("UDPSocketIp")) {
		UDPSocketIP = TempString;
	}
	else if (TempTag == TEXT("HttpBaseURL")) {
		HttpBaseURL = TempString;
	}
	else if (TempTag == TEXT("NationCode")) {

		NationCode = FCString::Atoi(*TempString);
		Infos.NationCode = NationCode;
	}

	return Infos;
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
		GConfig->GetString(TEXT("MY_INIT"), TEXT("Single_MapName"), Single_MapName, InIni);
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


const FName& UMyConfig::Get_ServerLevel()
{
	return PlayInitSettings.ServerIptoName;
}

const int32 UMyConfig::Get_NationCode()
{
	return PlayInitSettings.NationCode;
}
