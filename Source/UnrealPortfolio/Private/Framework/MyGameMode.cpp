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

void AMyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	Net_Mode = GetNetMode();
	if (Net_Mode == NM_Standalone) {
		// 패킷기반의 리플레이 뷰어 
	}
	else {

		//auto* mgr = GetGameInstance()->GetSubsystem<URtGameManager>();
		//mgr->Init_Game(MapName, Options, ErrorMessage, true);
	}
}

void AMyGameMode::InitGameState()
{
}

void AMyGameMode::PreInitializeComponents()
{
}

void AMyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
}

void AMyGameMode::PostLogin(APlayerController* NewPlayer)
{
}

void AMyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
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
}

FString AMyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	return FString();
}
