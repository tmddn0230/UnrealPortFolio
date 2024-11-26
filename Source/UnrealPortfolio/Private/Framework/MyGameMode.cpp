// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyGameMode.h"
// UE
#include "GameFramework/GameSession.h"

//Framework
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"         //including 'Kismet/Gameplaystatics.h'
#include "Framework/MyGameState.h"
#include "Framework/MyGameHUD.h"
#include "Framework/MySpectatorPawn.h"
//Manager
#include "Manager/MyGameManager.h"
//Common
#include "Common/MyBlueprintAssetPath.h"

AMyGameMode::AMyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass =        AMyGameState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass =      AMyPlayerState::StaticClass();
	HUDClass =              AMyGameHUD::StaticClass();
	SpectatorClass =        AMySpectatorPawn::StaticClass();

	const TCHAR* CharacterClass;
	CharacterClass = MyBlueprintAsset::MainCharacter;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(CharacterClass); // upcasting
	DefaultPawnClass = PlayerPawnClassFinder.Class;



}

AMyGameState* AMyGameMode::Get_GameState()
{
	return Cast<AMyGameState>(GameState);
}

AMyPlayerController* AMyGameMode::Get_PlayerController()
{
	return Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

AMyPlayerController* AMyGameMode::Get_PlayerController(int32 InPlayerId)
{
for (auto* pc : Trainee_Array) {
	if (pc) {
		if (auto* ps = pc->Get_PlayerState()) {
			if (ps)
			{
				if (ps->GetPlayerId() == InPlayerId) {
					return pc;
				}
			}
		}
	}
}

if (Control_Controller) {
	if (auto* ps = Control_Controller->Get_PlayerState()) {
		if (ps) {
			if (ps->GetPlayerId() == InPlayerId) {
				return Control_Controller;
			}
		}
	}
}

for (auto* pc : Observer_Array) {
	if (pc) {
		if (auto* ps = pc->Get_PlayerState()) {
			if (ps)
			{
				if (ps->GetPlayerId() == InPlayerId) {
					return pc;
				}
			}
		}
	}
}
	return nullptr;
}

AMyPlayerController* AMyGameMode::Get_TraineeController(int32 InPlayerId)
{
	for (auto* pc : Trainee_Array) {
		if (pc) {
			if (auto* ps = pc->Get_PlayerState()) {
				if (ps->GetPlayerId() == InPlayerId) {
					return pc;
				}
			}
		}
	}
	return nullptr;
}

bool AMyGameMode::Get_LoginInfo(AMyPlayerController* InPlayerController, FTraineeInfo_Login& OutPreLoginInfo)
{
	if (InPlayerController) {
		FString unique_id = InPlayerController->PlayerState->GetUniqueId().ToString();

		bool found = false;
		for (auto& it : Game_Login) {
			if (it.UniqueId.Equals(unique_id)) {
				OutPreLoginInfo = it;
				return true;
			}
		}
	}
	return false;
}

TArray<FDeviceInfo>& AMyGameMode::Get_DeviceArray()
{
	//if (auto* mgr = GetGameManager()) {
	//	if (auto* trainingmgr = mgr->TrngManager)
	//		return trainingmgr->Device_StatusArray;
	//}
	return FDeviceInfo::DefualtArray;
}

TArray<FTraineeInfo>& AMyGameMode::Get_TraineeArray()
{
	// TODO: insert return statement here
	return FTraineeInfo::DefualtArray;
}

void AMyGameMode::SC_DeviceInfo(int32 InId)
{
	if (auto* player = Get_PlayerController(InId)) {
		player->Client_Update_DeviceInfo(Get_DeviceArray());
	}
}

void AMyGameMode::SC_TraineeInfo(int32 InId)
{
	if (auto* player = Get_PlayerController(InId)) {
		player->Client_Update_TraineeInfo(Get_TraineeArray());
	}
}

AMyPlayerController* AMyGameMode::ExistDevice(APlayerController* NewPlayer)
{
	if (AMyPlayerController* player = Cast<AMyPlayerController>(NewPlayer)) {
		FDeviceInfo NewPlayerInfo;
		if (player->Get_DeviceInfo(NewPlayerInfo)) {
			for (auto* temp_player : Trainee_Array) {
				if (temp_player) {
					if (auto* ps = temp_player->Get_PlayerState()) {
						FDeviceInfo OutInfo;
						if (player->Get_DeviceInfo(OutInfo)) {
							if (OutInfo.Name.Equals(NewPlayerInfo.Name)) {
								return temp_player;
							}
						}
					}
				}
			}
		}
	}
	return nullptr;
}

TArray<AMyPlayerController*> AMyGameMode::ExistDeviceAll(APlayerController* NewPlayer)
{
	return TArray<AMyPlayerController*>();
}

bool AMyGameMode::Add_New_Player(AMyPlayerController* InPlayerController)
{
	return false;
}

bool AMyGameMode::Remove_Player(AMyPlayerController* InPlayerController)
{
	return false;
}

void AMyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	Net_Mode = GetNetMode();
	if (Net_Mode == NM_Standalone) {
		// 패킷기반의 리플레이 뷰어 
	}
	else {

		auto* mgr = GetGameInstance()->GetSubsystem<UMyGameManager>();
		mgr->Init_Game(MapName, Options, ErrorMessage, true);
	}
}

void AMyGameMode::InitGameState()
{
	Super::InitGameState();
}

void AMyGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	++ConnectCount;

	if (Net_Mode == NM_Standalone) {
	}
	else {
		if (auto* mgr = GetGameInstance()->GetSubsystem<UMyGameManager>()) {
			if (Pre_Login(Options, Address, UniqueId, ErrorMessage, mgr->Get_Current_GameMode()) == false) {
				FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
				return;
			}
		}
	}
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

bool AMyGameMode::Pre_Login(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage, EUP_GameMode InGameMode)
{
	//``Pre login 시 발생 정보를 저장합니다.
	FString str_Options = FString::Printf(TEXT("Options : %s"), *Options);
	FString str_Address = FString::Printf(TEXT("Address : %s"), *Address);

	// Save Player Information.
	FTraineeInfo_Login info;
	info.Set_Options(Options);
	info.Address = Address;
	info.UniqueId = UniqueId.ToString();
	info.ErrorMessage = ErrorMessage;
	info.TimeStr = FDateTime::Now().ToString();

	// This is Filling Code. We get needed data from rtsettings.ini 
	// This Step, we need Only Device data for matching Trainee Login

	if (InGameMode == EUP_GameMode::E_MultiPlay) {
		Game_Login.Add(info);

		if (Game_Login_Try_ClientList.Contains(info.UniqueId) == false) {
			Game_Login_Try_ClientList.Add(info.UniqueId, info);
		}
		else {
            // Exist User : 이미 해당 ID 의 플레이어가 존재한다.
		}
	}

	return true;
}

void AMyGameMode::PostLogin(APlayerController* NewPlayer)
{
	UWorld* World = GetWorld();

	// update player count
	if (MustSpectate(NewPlayer))
	{
		Num_Spectators++;
	}
	else if (World->IsInSeamlessTravel() || NewPlayer->HasClientLoadedCurrentWorld())
	{
		Num_Players++;
	}
	else
	{
		Num_TravellingPlayers++;
	}

	// save network address for re-associating with reconnecting player, after stripping out port number
	FString Address = NewPlayer->GetPlayerNetworkAddress();
	int32 pos = Address.Find(TEXT(":"), ESearchCase::CaseSensitive);
	NewPlayer->PlayerState->SavedNetworkAddress = (pos > 0) ? Address.Left(pos) : Address;

	// check if this player is reconnecting and already has PlayerState
	FindInactivePlayer(NewPlayer);

	if (IsServerMode()) {
		if (auto* pc = Cast<AMyPlayerController>(NewPlayer)) {
			if (NewPlayer->IsLocalController()) {
				if (auto* player_state = Cast<AMyPlayerState>(NewPlayer->PlayerState)) {
					player_state->PlayType = EUP_PlayType::E_Server;
					pc->Set_Spectator(true);
				}
			}
			else { // case of client
				if (auto* player_state = Cast<AMyPlayerState>(NewPlayer->PlayerState)) {
					if (auto* mgr = GetGameInstance()->GetSubsystem<UMyGameManager>()) {
						FTraineeInfo_Login out_info;
						if (Get_LoginInfo(pc, out_info))
						{
							// Deliver Play Type : Server.., Trainee
							player_state->PlayType = out_info.PlayerType;

							if (player_state->PlayType == EUP_PlayType::E_Control) {
								pc->Set_Spectator(true);
							}
							else if (player_state->PlayType == EUP_PlayType::E_Client) {
								pc->Set_Spectator(false);
							}
							else if (player_state->PlayType == EUP_PlayType::E_Replay) {
								pc->Set_Spectator(true);
							}
							else {
								pc->Set_Spectator(false);
							}
						}
						else {
							// None PlayType!!
						}
					}
				}
				else {
				     // Login Fail!
				}
			}
		}
		//OnPostLogin_Delegate.Broadcast(this, NewPlayer);
	}
	Super::PostLogin(NewPlayer);
}

void AMyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	auto* game_manager = GetGameManager();
	if (game_manager && game_manager->Is_Play_Type(EUP_PlayType::E_Server)) {

		//Handling duplicate device logins
		auto exist_devices = ExistDeviceAll(NewPlayer);
		for (auto* it : exist_devices) {
			GameSession->KickPlayer(it, FText::FromString(TEXT("Multiplay Login")));
		}

		if (AMyPlayerController* player = Cast< AMyPlayerController>(NewPlayer)) {
			if (Add_New_Player(player)) {
				SC_DeviceInfo();
			}
		}
	}
}

void AMyGameMode::Logout(AController* Exiting)
{
}

void AMyGameMode::StartPlay()
{
}

void AMyGameMode::BeginPlay()
{
}

void AMyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FString AMyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString res = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	if (auto* PlayerController = Cast<AMyPlayerController>(NewPlayerController)) {
		if (auto* PlayerState = PlayerController->Get_PlayerState()) {
			PlayerState->Set_LoginInfo(Options, NewPlayerController->IsLocalController());
		}
	}
	return res;
}
