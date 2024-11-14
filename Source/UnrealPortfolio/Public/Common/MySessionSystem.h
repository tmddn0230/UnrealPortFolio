// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// Interface
#include "Interfaces/OnlineSessionInterface.h" // Online Module
#include "Subsystems/GameInstanceSubsystem.h"
#include "MySessionSystem.generated.h"

/*
내부 네트워크를 사용하고 있어 사실 세션은 작동하지 않는다.
OnlineSubsystemNull의 한계
나중에 CreateSession 만 사용
레퍼런스가 있어서 작성만 해두는 클래스
*/

// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogMySessionSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

UCLASS()
class UNREALPORTFOLIO_API UMySessionSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public: 
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch>   LastSessionSearch;

	// Create Session
	FCSOnCreateSessionComplete        CreateSessionHandle;    
	FOnCreateSessionCompleteDelegate  CreateSessionCompleteDelegate;
	FDelegateHandle                   CreateSessionCompleteDelegateHandle;
	// Update Session				 
	FCSOnUpdateSessionComplete        UpdateSessionHandle;
	FOnUpdateSessionCompleteDelegate  UpdateSessionCompleteDelegate;
	FDelegateHandle                   UpdateSessionCompleteDelegateHandle;
	// Start  Session				 
	FCSOnStartSessionComplete         StartSessionHandle;
	FOnStartSessionCompleteDelegate   StartSessionCompleteDelegate;
	FDelegateHandle                   StartSessionCompleteDelegateHandle;
	// End  Session					 
	FCSOnEndSessionComplete           EndSessionHandle;
	FOnEndSessionCompleteDelegate     EndSessionCompleteDelegate;
	FDelegateHandle                   EndSessionCompleteDelegateHandle;
	// Destroy Session
	FCSOnDestroySessionComplete       DestroySessionHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle                   DestroySessionCompleteDelegateHandle;
	// Find Session
	FCSOnFindSessionsComplete         FindSessionsHandle;
	FOnFindSessionsCompleteDelegate   FindSessionsCompleteDelegate;
	FDelegateHandle                   FindSessionsCompleteDelegateHandle;
	// Join Session
	FCSOnJoinSessionComplete          JoinGameSessionHandle;
	FOnJoinSessionCompleteDelegate    JoinSessionCompleteDelegate;
	FDelegateHandle                   JoinSessionCompleteDelegateHandle;


public:
	UMySessionSystem();

	// Session Flow
	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);
	void UpdateSession();
	void StartSession();
	void EndSession();
	void DestroySession();
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	void JoinGameSession_EF(const FOnlineSessionSearchResult& SessionResult);

	// Binding 
protected:
	UFUNCTION() void OnCreateSessionCompleted(FName SessionName, bool Successful);
	UFUNCTION() void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	UFUNCTION() void OnStartSessionCompleted(FName SessionName, bool Successful);
	UFUNCTION() void OnEndSessionCompleted(FName SessionName, bool Successful);
	UFUNCTION() void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:
	bool TryTravelToCurrentSession();

};
