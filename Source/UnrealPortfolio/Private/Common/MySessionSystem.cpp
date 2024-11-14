// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MySessionSystem.h"
#include "OnlineSubsystemUtils.h"

DEFINE_LOG_CATEGORY(LogMySessionSystem);

/*
* 1. GameManager::Start_Server ���� CreateSession �� true ���� �־� ��������
* 2. OnCreateSessionCompleted ���� true�� �־� CreateComplete ��������Ʈ�� ������ ȣ��
*/

UMySessionSystem::UMySessionSystem()
	:CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted))
{

}

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

	/* EOS ���� �ڵ�
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

	// EOS ���� �ڵ�
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
