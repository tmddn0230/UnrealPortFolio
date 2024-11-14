// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyGameModeBase.h"

// Unreal Basic
#include "Kismet/GameplayStatics.h"
// Framework
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"
#include "Framework/MyGameState.h"

DEFINE_LOG_CATEGORY(LogGMB);

AMyGameModeBase::AMyGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// initialize value
	InactivePlayerStateLifeSpan = 300.f;   // 로그아웃 이후 비활성화된 플레이어 스테이트가 머무르는 시간
	//bDelayed_Start = false;
	//Min_RespawnDelay = 1.0f;
	Max_InactivePlayers = 20;
}

void AMyGameModeBase::AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC)
{
	check(PlayerState)
		UWorld* LocalWorld = GetWorld();

	// don't store if it's an old PlayerState from the previous level or if it's a spectator... or if we are shutting down
	if (!PlayerState->IsFromPreviousLevel() && !MustSpectate(PC) && !LocalWorld->bIsTearingDown)
	{
		APlayerState* const NewPlayerState = PlayerState->Duplicate();
		if (NewPlayerState)
		{
			// Side effect of Duplicate() adding PlayerState to PlayerArray (see APlayerState::PostInitializeComponents)
			GameState->RemovePlayerState(NewPlayerState);
	
			// make PlayerState inactive
			NewPlayerState->SetReplicates(false);
	
			// delete after some time
			NewPlayerState->SetLifeSpan(InactivePlayerStateLifeSpan);
	
			// On console, we have to check the unique net id as network address isn't valid
			const bool bIsConsole = !PLATFORM_DESKTOP;
			// Assume valid unique ids means comparison should be via this method
			const bool bHasValidUniqueId = NewPlayerState->GetUniqueId().IsValid();
			// Don't accidentally compare empty network addresses (already issue with two clients on same machine during development)
			const bool bHasValidNetworkAddress = !NewPlayerState->SavedNetworkAddress.IsEmpty();
			const bool bUseUniqueIdCheck = bIsConsole || bHasValidUniqueId;
	
			// make sure no duplicates, 추가 전 확실하게 지움
			for (int32 Idx = 0; Idx < InactivePlayerArray.Num(); ++Idx)
			{
				APlayerState* const CurrentPlayerState = InactivePlayerArray[Idx];
				if ((CurrentPlayerState == nullptr) || IsValid(CurrentPlayerState))
				{
					// already destroyed, just remove it
					InactivePlayerArray.RemoveAt(Idx, 1);
					Idx--;
				}
				else if ((!bUseUniqueIdCheck && bHasValidNetworkAddress && (CurrentPlayerState->SavedNetworkAddress == NewPlayerState->SavedNetworkAddress))
					|| (bUseUniqueIdCheck && (CurrentPlayerState->GetUniqueId() == NewPlayerState->GetUniqueId())))
				{
					// destroy the playerstate, then remove it from the tracking
					CurrentPlayerState->Destroy();
					InactivePlayerArray.RemoveAt(Idx, 1);
					Idx--;
				}
			}
			InactivePlayerArray.Add(NewPlayerState);
	
			// make sure we dont go over the maximum number of inactive players allowed
			if (InactivePlayerArray.Num() > Max_InactivePlayers)
			{
				int32 const NumToRemove = InactivePlayerArray.Num() - Max_InactivePlayers;
	
				// destroy the extra inactive players
				for (int Idx = 0; Idx < NumToRemove; ++Idx)
				{
					APlayerState* const PS = InactivePlayerArray[Idx];
					if (PS != nullptr)
					{
						PS->Destroy();
					}
				}
	
				// and then remove them from the tracking array
				InactivePlayerArray.RemoveAt(0, NumToRemove);
			}
		}
	}
}

bool AMyGameModeBase::FindInactivePlayer(APlayerController* PC)   // 플레이어 재접속
{
	check(PC && PC->PlayerState);
	// don't bother for spectators
	if (MustSpectate(PC))
	{
		return false;
	}

	// On console, we have to check the unique net id as network address isn't valid
	const bool bIsConsole = !PLATFORM_DESKTOP;
	// Assume valid unique ids means comparison should be via this method
	const bool bHasValidUniqueId = PC->PlayerState->GetUniqueId().IsValid();
	// Don't accidentally compare empty network addresses (already issue with two clients on same machine during development)
	const bool bHasValidNetworkAddress = !PC->PlayerState->SavedNetworkAddress.IsEmpty();
	const bool bUseUniqueIdCheck = bIsConsole || bHasValidUniqueId;

	const FString NewNetworkAddress = PC->PlayerState->SavedNetworkAddress;
	const FString NewName = PC->PlayerState->GetPlayerName();
	for (int32 i = 0; i < InactivePlayerArray.Num(); i++)
	{
		APlayerState* CurrentPlayerState = InactivePlayerArray[i];
		if ((CurrentPlayerState == nullptr) || !IsValid(CurrentPlayerState))
		{
			InactivePlayerArray.RemoveAt(i, 1);
			i--;
		}
		else if ((bUseUniqueIdCheck && (CurrentPlayerState->GetUniqueId() == PC->PlayerState->GetUniqueId())) ||
			(!bUseUniqueIdCheck && bHasValidNetworkAddress && (FCString::Stricmp(*CurrentPlayerState->SavedNetworkAddress, *NewNetworkAddress) == 0) && (FCString::Stricmp(*CurrentPlayerState->GetPlayerName(), *NewName) == 0)))
		{
			// found it!
			// 이전의 기록을 삭제하고 새로 초기화
			// 유효한 고유 ID가 있는지, 유효한 네트워크 주소로 검색한것!
			APlayerState* OldPlayerState = PC->PlayerState;
			PC->PlayerState = CurrentPlayerState;
			PC->PlayerState->SetOwner(PC);
			PC->PlayerState->SetReplicates(true);
			PC->PlayerState->SetLifeSpan(0.0f);
			// 가지고 있던 이전 PS 정보 업데이트
			OverridePlayerState(PC, OldPlayerState);
			GameState->AddPlayerState(PC->PlayerState);
			InactivePlayerArray.RemoveAt(i, 1);
			OldPlayerState->SetIsInactive(true);
			// Set the uniqueId to nullptr so it will not kill the player's registration 
			// in UnregisterPlayerWithSession()
			OldPlayerState->SetUniqueId(nullptr);
			OldPlayerState->Destroy();
			PC->PlayerState->OnReactivated();
			return true;
		}
	}
	return false;
}

void AMyGameModeBase::OverridePlayerState(APlayerController* PC, APlayerState* OldPlayerState)
{
	PC->PlayerState->DispatchOverrideWith(OldPlayerState);
}


void AMyGameModeBase::Post_Login_Sub(APlayerController* NewPlayer)
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

	// save network address for re-associating with reconnecting player, after stripping out port number, 포트번호는 동적으로 주어지기 때문에 매번 다름. 그래서 제거
	FString Address = NewPlayer->GetPlayerNetworkAddress();
	int32 pos = Address.Find(TEXT(":"), ESearchCase::CaseSensitive);
	NewPlayer->PlayerState->SavedNetworkAddress = (pos > 0) ? Address.Left(pos) : Address;

	// check if this player is reconnecting and already has PlayerState
	FindInactivePlayer(NewPlayer);
}

bool AMyGameModeBase::IsServerMode()
{
	auto mode = GetNetMode();
	if (mode == ENetMode::NM_DedicatedServer || mode == ENetMode::NM_ListenServer)
		return true;
	return false;
}
