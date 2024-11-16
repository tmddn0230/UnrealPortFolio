// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Common
#include "Common/MyCommon.h"
#include "MyGameManager.generated.h"


UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogGameManager, Log, All);
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogPlayerInfo, Log, All);

// Framework
class AMyPlayerController;
class AMyPlayerState;
class AMyGameState;


UCLASS()
class UNREALPORTFOLIO_API UMyGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	EUP_GameMode Current_GameMode = EUP_GameMode::E_None;
	EUP_PlayType Play_Type = EUP_PlayType::E_None;
	
	bool bListenServer = false;
	bool bSinglePlay;
	bool bStartModeRun = false;
	bool bInitialized = false;

public:
	TArray<IConsoleObject*> AuditCmds;     // 콘솔 명령용 

public:
	UMyGameManager();

	//Get
	EUP_PlayType Get_PlayType() { return Play_Type; }

	// Main Flow
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool bGamemode);
	bool IsInitialzied();

	void Quit_Program();

	void Start_PlayMode();

	/*
	* 메뉴에서 실행할 때 밑의 함수들을 바로 호출한다.
	*/
	UFUNCTION(BlueprintCallable) void Start_SingleMode();
	UFUNCTION(BlueprintCallable) void Start_Server();
	UFUNCTION(BlueprintCallable) void Start_Client (const FString& InDeviceName , const FString& InDeviceId);
	UFUNCTION(BlueprintCallable) void Start_Control();
	UFUNCTION(BlueprintCallable) void Start_Observer(const FString& InDeviceName);
	UFUNCTION(BlueprintCallable) void Start_Replay (const FString& TrainingID, const FString& UserID, const FString& ScenarioName);
};
