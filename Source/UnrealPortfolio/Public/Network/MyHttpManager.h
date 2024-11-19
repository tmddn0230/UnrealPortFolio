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
	FString WebUrl = "";
	FString ScenarioUrl = "";
	FString ReplayUploadUrl = "";
	FString ReplayDownloadUrl = "";
	FString WebLogUrl = "";


	// Delegate Handle
	FRecvHandleLoginCheck Handle_LoginCheck;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	// Request
	void RequestWeb(const FString& URL, const EWebProtocolType& protocol);
	void RequestWeb_OneParam(const FString& URL, const EWebProtocolType& protocol, const FString& ParamName, const FString& param1);
	void RequestWeb_TwoParam(const FString& URL, const EWebProtocolType& protocol, const FString& ParamName1, const FString& param1, const FString& ParamName2, const FString& param2);

	// Respond
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponse_OneParam(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponse_TwoParam(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// Case
	void ProcessbyCase(int32 protocol, TSharedPtr<FJsonObject> ResultObject, TSharedRef<TJsonReader<>> resultReader);

	void Login(const FString& InID, const FString& InPassword);

};
