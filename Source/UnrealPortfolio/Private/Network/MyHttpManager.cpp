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
	HttpBase         = UMyConfig::HttpBaseURL;
	MainURL          = HttpBase + "main.aspx";
	TemplateDownURL  = HttpBase + "download.aspx";
	LogURL           = HttpBase + "logmain.aspx";
	VoiceUploadURL   = HttpBase + "upload_voice.aspx";
	VoiceDownloadURL = HttpBase + "download_voice.aspx";
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UMyHttpManager::SetRequestOptions(const FString& URL)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	return Request;
}

FString UMyHttpManager::GetContentString(const TSharedRef<FJsonObject>& requestObj )
{
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(requestObj, Writer);

	return RequestBody;
}


void UMyHttpManager::Login(const FString& InID, const FString& InPassword)
{
	// Main
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(MainURL);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &UMyHttpManager::OnResponse_Login);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("protocol", FString::FromInt((int32)EWebProtocolType::webLogin));
	RequestObj->SetStringField("account", InID);
	RequestObj->SetStringField("pass", InPassword);

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogMyHttpManager, Warning, TEXT("Login Request Called"));
}

void UMyHttpManager::OnResponse_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

		if (FJsonSerializer::Deserialize(resultReader, ResultObject))
		{
			// Get Login Result
			int32 ResultValue = ResultObject->GetIntegerField(TEXT("RESULT"));
			// Process Login On Widget
			Handle_LoginCheck.Broadcast(ResultValue);
		}
	}
}