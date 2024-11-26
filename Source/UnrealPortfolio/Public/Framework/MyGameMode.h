// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MyGameModeBase.h"
// RpcCommon
#include "Common/MyRpcCommon.h"
#include "MyGameMode.generated.h"

class AMyPlayerController;
class AMyPlayerState;
class AMyGameState;
class UMyGameManager;

UCLASS()
class UNREALPORTFOLIO_API AMyGameMode : public AMyGameModeBase
{
	GENERATED_BODY()
// Delegate

private:

	ENetMode Net_Mode;
	int32 ConnectCount;         // Number of Clients
	static int32 LoginCount;
	TArray<AMyPlayerController*> Trainee_Array;
	TArray<AMyPlayerController*> Observer_Array;
	AMyPlayerController*         Control_Controller;

	// Save File 에 저장하여 재접속 등 예외처리에 사용.
	TArray<FTraineeInfo_Login>        Game_Login;
	TMap<FString, FTraineeInfo_Login> Game_Login_Try_ClientList;

public:
	AMyGameMode(const FObjectInitializer& ObjectInitializer);

	// Get
	AMyGameState*         Get_GameState();
	AMyPlayerController*  Get_PlayerController();
	AMyPlayerController*  Get_PlayerController(int32 InPlayerId); 
	AMyPlayerController*  Get_TraineeController(int32 InPlayerId); 
	bool                  Get_LoginInfo(AMyPlayerController* InPlayerController, FTraineeInfo_Login& OutPreLoginInfo);
	TArray<FDeviceInfo>&  Get_DeviceArray();
	TArray<FTraineeInfo>& Get_TraineeArray();
	UMyGameManager*       Get_GameManager();
	// Update
	void SC_DeviceInfo();
	void SC_DeviceInfo(int32 InId);
	void SC_TraineeInfo();
	void SC_TraineeInfo(int32 InId);

	// Add, KickPlayer
	AMyPlayerController*         ExistDevice(APlayerController* NewPlayer);
	TArray<AMyPlayerController*> ExistDeviceAll(APlayerController* NewPlayer);
	bool Add_New_Player(AMyPlayerController* InPlayerController);
	bool Remove_Player(AMyPlayerController* InPlayerController);

	// Overriding
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;
	virtual void PreInitializeComponents() override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	bool         Pre_Login(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage, EUP_GameMode InGameMode);
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartPlay() override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

};
