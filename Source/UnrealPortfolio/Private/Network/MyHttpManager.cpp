// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/MyHttpManager.h"
// Common
#include "Common/MyConfig.h"

// Log
DEFINE_LOG_CATEGORY(LogMyHttpManager);

void UMyHttpManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Init
	WebUrl            = UMyConfig::WebServerUrl;
	ScenarioUrl       = UMyConfig::WebScenarioUrl;
	WebLogUrl         = UMyConfig::WebLogUrl;
	ReplayUploadUrl   = UMyConfig::WebReplayUploadUrl;
	ReplayDownloadUrl = UMyConfig::WebReplayDownloadUrl;
}

void UMyHttpManager::RequestWeb(const FString& URL, const EWebProtocolType& protocol)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = FHttpModule::Get().CreateRequest();

	UserRequest->OnProcessRequestComplete().BindUObject(this, &UMyHttpManager::OnResponse);
	UserRequest->SetURL(URL);
	UserRequest->SetVerb("POST");
	UserRequest->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("protocol", FString::FromInt((int32)protocol)); 

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	UserRequest->SetContentAsString(RequestBody);
	UserRequest->ProcessRequest();
}

void UMyHttpManager::RequestWeb_OneParam(const FString& URL, const EWebProtocolType& protocol, const FString& ParamName, const FString& param1)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = FHttpModule::Get().CreateRequest();

	UserRequest->OnProcessRequestComplete().BindUObject(this, &UMyHttpManager::OnResponse_OneParam);
	UserRequest->SetURL(URL);
	UserRequest->SetVerb("POST");
	UserRequest->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("protocol", FString::FromInt((int32)protocol));
	RequestObj->SetStringField(ParamName, param1);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	UserRequest->SetContentAsString(RequestBody);
	UserRequest->ProcessRequest();
}

void UMyHttpManager::RequestWeb_TwoParam(const FString& URL, const EWebProtocolType& protocol, const FString& ParamName1, const FString& param1, const FString& ParamName2, const FString& param2)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = FHttpModule::Get().CreateRequest();

	UserRequest->OnProcessRequestComplete().BindUObject(this, &UMyHttpManager::OnResponse_OneParam);
	UserRequest->SetURL(URL);
	UserRequest->SetVerb("POST");
	UserRequest->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("protocol", FString::FromInt((int32)protocol));
	RequestObj->SetStringField(ParamName1, param1);
	RequestObj->SetStringField(ParamName2, param2);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	UserRequest->SetContentAsString(RequestBody);
	UserRequest->ProcessRequest();
}



void UMyHttpManager::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response == nullptr) return;
	if (!bWasSuccessful) return;

	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// Try to convert string to json object. Output goes in RetJsonObject
	if (FJsonSerializer::Deserialize(jsonReader, JsonObject))
	{
		int32 protocol;
		if (JsonObject->TryGetNumberField(TEXT("protocol"), protocol))
		{
			UE_LOG(LogMyHttpManager, Log, TEXT("Protocol : %d"), protocol);
		}

		TSharedPtr<FJsonObject> ResultObject;
		FString Result = JsonObject->GetStringField("result");
		TSharedRef<TJsonReader<>> resultReader = TJsonReaderFactory<>::Create(Result);

		ProcessbyCase(protocol, ResultObject, resultReader);
	}
}

void UMyHttpManager::OnResponse_OneParam(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void UMyHttpManager::OnResponse_TwoParam(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}




void UMyHttpManager::ProcessbyCase(int32 protocol, TSharedPtr<FJsonObject> ResultObject, TSharedRef<TJsonReader<>> resultReader)
{
	switch (protocol)
	{
	case 11:  // Login
		if (FJsonSerializer::Deserialize(resultReader, ResultObject))
		{
			// Get Login Result
			int32 ResultValue = ResultObject->GetIntegerField(TEXT("RESULT"));
		    // Process Login On Widget
			Handle_LoginCheck.Broadcast(ResultValue);
		}
		break;
	}
}


void UMyHttpManager::Login(const FString& InID, const FString& InPassword)
{
	RequestWeb_TwoParam(WebUrl, EWebProtocolType::webLogin, "account", InID, "pass", InPassword);
}
