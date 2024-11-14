// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// Interface
#include "Interfaces/OnlineSessionInterface.h" // Online Module
#include "Subsystems/GameInstanceSubsystem.h"
#include "MySessionSystem.generated.h"

// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogMySessionSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);

UCLASS()
class UNREALPORTFOLIO_API UMySessionSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public: 
	// for EOS
	// TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	
	// Create Session
	FCSOnCreateSessionComplete       CreateSessionHandle;    
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle                  CreateSessionCompleteDelegateHandle;
	// Update Session
	FCSOnUpdateSessionComplete       UpdateSessionHandle;
	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle                  UpdateSessionCompleteDelegateHandle;
	// Start  Session
	FCSOnStartSessionComplete        StartSessionHandle;
	FOnStartSessionCompleteDelegate  StartSessionCompleteDelegate;
	FDelegateHandle                  StartSessionCompleteDelegateHandle;



public:
	UMySessionSystem();

	// Session Flow
	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);


	// Binding 
protected:
	UFUNCTION()
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

};
