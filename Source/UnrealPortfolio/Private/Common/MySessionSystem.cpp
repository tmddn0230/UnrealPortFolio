// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MySessionSystem.h"
#include "OnlineSubsystemUtils.h"

DEFINE_LOG_CATEGORY(LogMySessionSystem);

/*
* OnLineSystem에서의 활용
* 1. GameManager::Start_Server 에서 CreateSession 을 true 값을 주어 실행
* 2. OnCreateSessionCompleted 에서 true를 넣어 CreateComplete 델리게이트를  호출
*/

UMySessionSystem::UMySessionSystem()
	:CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted)),
	 UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted)),
	 StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted)),
	 EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted)),
	 DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
{

}

///////////////////// 
///    Create    ///
/////////////////// 
void UMySessionSystem::CreateSession(int32 NumPublicConnections, bool IsLANMatch)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		// Create Session Fail !!
		CreateSessionHandle.Broadcast(false);
		UE_LOG(LogMySessionSystem, Error, TEXT("Failed Create Session!!!"));
		return;
	}

	/* EOS 전용 코드
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = IN NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUseLobbiesVoiceChatIfAvailable = true;

	LastSessionSettings->Set(SETTING_MAPNAME, FString("RT"), EOnlineDataAdvertisementType::ViaOnlineService);
	*/

	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	// EOS 전용 코드
	//const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	//{
	//	CreateSessionHandle.Broadcast(false);
	//}

}

void UMySessionSystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	CreateSessionHandle.Broadcast(Successful);
}


///////////////////// 
///    Update    ///
/////////////////// 
void UMySessionSystem::UpdateSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		UpdateSessionHandle.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	updatedSessionSettings->Set(SETTING_MAPNAME, FString("Updated Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle =
		sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!sessionInterface->UpdateSession(NAME_GameSession, *updatedSessionSettings))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

		UpdateSessionHandle.Broadcast(false);
	}
	else
	{
		LastSessionSettings = updatedSessionSettings;
	}
}

void UMySessionSystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	UpdateSessionHandle.Broadcast(Successful);
}


///////////////////// 
///    Start     ///
/////////////////// 
void UMySessionSystem::StartSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		StartSessionHandle.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle =
		sessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		StartSessionHandle.Broadcast(false);
	}
}

void UMySessionSystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	StartSessionHandle.Broadcast(Successful);
}


///////////////////// 
///     End      ///
/////////////////// 
void UMySessionSystem::EndSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		EndSessionHandle.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle =
		sessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if (!sessionInterface->EndSession(NAME_GameSession))
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

		EndSessionHandle.Broadcast(false);
	}
}

void UMySessionSystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	EndSessionHandle.Broadcast(Successful);
}


///////////////////// 
///   Destroy    ///
/////////////////// 
void UMySessionSystem::DestroySession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		DestroySessionHandle.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle =
		sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!sessionInterface->DestroySession(NAME_GameSession))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		DestroySessionHandle.Broadcast(false);
	}
}

void UMySessionSystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	DestroySessionHandle.Broadcast(Successful);
}


///////////////////// 
///     Find     ///
/////////////////// 
void UMySessionSystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		FindSessionsHandle.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle =
		sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		FindSessionsHandle.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UMySessionSystem::OnFindSessionsCompleted(bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		FindSessionsHandle.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	FindSessionsHandle.Broadcast(LastSessionSearch->SearchResults, Successful);
}


///////////////////// 
///     Join     ///
/////////////////// 
void UMySessionSystem::JoinGameSession_EF(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		JoinGameSessionHandle.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle =
		sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		JoinGameSessionHandle.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMySessionSystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	JoinGameSessionHandle.Broadcast(Result);
}




bool UMySessionSystem::TryTravelToCurrentSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return false;
	}

	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}



