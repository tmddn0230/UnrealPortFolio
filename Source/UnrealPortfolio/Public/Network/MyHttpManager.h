// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Http
#include "http.h"
// Protocol
#include "UPCore/Public/MyProtocol.h"
// Common
#include "Common/MyRpcCommon.h"

#include "MyHttpManager.generated.h"

// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogMyHttpManager, Log, All);

// Delegate
DECLARE_MULTICAST_DELEGATE_OneParam(FRecvHandleLoginCheck, int32 Result);


UCLASS()
class UNREALPORTFOLIO_API UMyHttpManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// URL
	FString HttpBase = "";
	FString MainURL = "";
	FString TemplateDownURL = "";
	FString VoiceUploadURL = "";
	FString VoiceDownloadURL = "";
	FString LogURL = "";


	// Delegate Handle
	FRecvHandleLoginCheck Handle_LoginCheck;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> SetRequestOptions(const FString& URL);
	FString                                       GetContentString(const TSharedRef<FJsonObject>& requestObj);

	// Request
	void Login(const FString& InID, const FString& InPassword);

	// Respond
	void OnResponse_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


};
