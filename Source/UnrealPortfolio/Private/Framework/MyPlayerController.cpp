// Fill out your copyright notice in the Description page of Project Settings.

// Framework
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"
#include "Framework/MyGameMode.h"
#include "Framework/MyGameHUD.h"

// Widget
#include "UI/MyUI_PrimaryLayout.h"
#include "UI/MyUI_PageBase.h"



AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMyPlayerController::Init_PlayType()
{
	if (PlayerState)
	{
		if (auto* ps = Get_PlayerState()) {
			ps->CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::Handle_Changed_DeviceInfo));
			ENetMode NetMode = GetNetMode();

			if (IsLocalPlayerController()) {
				auto netmode = GetNetMode();
				if (netmode == ENetMode::NM_Standalone) {
					if (ps->PlayType == EUP_PlayType::E_Replay) {
						// TODO
						// 리플레이 시작시 할 일
					}
				}
				else {
					switch (ps->PlayType) {

					case EUP_PlayType::E_Client:
						// TODO
						// 클라이언트 시작시 할 일
						break;
					case EUP_PlayType::E_Server: {
						// TODO
						// 서버 시작시 할 일
					}break;
					case EUP_PlayType::E_Control: {
						// 관제 시스템 시작시 할 일
						//Server_Send_DeviceInfo(Get_Id());
						//Server_Send_TraineeInfo(Get_Id());
					}break;

					}
				}
			}
		}
	}
}

AMyGameMode* AMyPlayerController::Get_GameMode()
{
	UWorld* World = GetWorld();
	check(World);
	return Cast<AMyGameMode>(World->GetAuthGameMode());
}

AMyGameHUD* AMyPlayerController::Get_HUD()
{
	auto* res = Cast<AMyGameHUD>(GetHUD());
	return res;
}

AMyPlayerState* AMyPlayerController::Get_PlayerState()
{
	return Cast<AMyPlayerState>(PlayerState);
}

EUP_PlayType AMyPlayerController::Get_PlayType()
{
	if (auto* playState = Get_PlayerState()) {
		return playState->PlayType;
	}
	return EUP_PlayType::E_None;
}

TWeakObjectPtr<UMyUI_PrimaryLayout> AMyPlayerController::Get_PrimaryLayout()
{
	// TODO 

	return TWeakObjectPtr<UMyUI_PrimaryLayout>();
}


void AMyPlayerController::Set_Spectator(bool val)
{
	if (auto* ps = GetPlayerState<AMyPlayerState>()) {
		if (val) {
			ps->SetIsOnlyASpectator(val);
			ChangeState(NAME_Spectating);
		}
		else {
			ps->SetIsOnlyASpectator(val);
			ChangeState(NAME_Playing);
		}
	}
}

void AMyPlayerController::Set_DeviceInfo(const TArray<FDeviceInfo>& InInfo)
{
	Update_DeviceList_Delegate.Broadcast(this, InInfo);
}

void AMyPlayerController::Set_TraineeInfo(const TArray<FTraineeInfo>& InInfo)
{
	Update_TraineeList_Delegate.Broadcast(this, InInfo);
}



UMyUI_PageBase* AMyPlayerController::Open_Page(const FName& InName)
{
	if (auto* hud = Get_HUD()) {
		return hud->Open_Page(InName, false);
	}
	else {
		TWeakObjectPtr<UMyUI_PrimaryLayout> primary_layout = Get_PrimaryLayout();
		if (primary_layout.IsValid()) {
			primary_layout.Get()->Open_Page(InName, false, this, nullptr);
		}
	}
	return nullptr;
}

UMyUI_PageBase* AMyPlayerController::Open_Popup(const FName& InName)
{
	if (auto* hud = Get_HUD()) {
		return hud->Open_Page(InName, true);
	}
	else {
		TWeakObjectPtr<UMyUI_PrimaryLayout> primary_layout = Get_PrimaryLayout();
		if (primary_layout.IsValid()) {
			primary_layout.Get()->Open_Page(InName, true, this, nullptr);
		}
	}
	return nullptr;
}

bool AMyPlayerController::Close_Popup(UMyUI_PageBase* InPage)
{
	if (auto* hud = Get_HUD()) {
		return hud->Close_Popup(InPage);
	}
	else {
		TWeakObjectPtr<UMyUI_PrimaryLayout> primary_layout = Get_PrimaryLayout();
		if (primary_layout.IsValid()) {
			primary_layout.Get()->Close_Popup(InPage);
		}
	}
	return false;
}

void AMyPlayerController::Handle_Changed_DeviceInfo()
{
	if (auto* ps = Get_PlayerState()) {
		OnChanged_DeviceInfo_delegate.Broadcast(ps->Get_PlayerInfo(), ps);
	}
}

void AMyPlayerController::CallBeforeBindingToPlayerState()
{
	Handle_Changed_DeviceInfo();
}



// Server 함수를 통해 GameMode에 데이터 동기화
void AMyPlayerController::Server_Send_DeviceInfo_Implementation(int32 InId)
{
	if (InId == 0)
		return;

	if (auto* game_mode = Get_GameMode()) {
	    game_mode->SC_DeviceInfo(InId);
	}
}

void AMyPlayerController::Server_Send_TraineeInfo_Implementation(int32 InId)
{
	if (InId == 0)
		return;

	if (auto* game_mode = Get_GameMode()) {
		game_mode->SC_TraineeInfo(InId);
	}
}

void AMyPlayerController::Client_Update_DeviceInfo_Implementation(const TArray<FDeviceInfo>& InInfo)
{
	Set_DeviceInfo(InInfo);
}

void AMyPlayerController::Client_Update_TraineeInfo_Implementation(const TArray<FTraineeInfo>& InInfo)
{
	Set_TraineeInfo(InInfo);
}

