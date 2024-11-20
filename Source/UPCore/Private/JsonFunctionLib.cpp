// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonFunctionLib.h"
// File I/O
#include "FileIOFunctionLib.h"

#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities

template <typename StructType>
void UJsonFunctionLib::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput) {
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void UJsonFunctionLib::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}


TSharedPtr<FJsonObject> UJsonFunctionLib::ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	//Try to read file 
	FString JsonString = UFileIOFunctionLib::ReadStringFromFile(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> RetJsonObject;

	// Try to convert string to json object. Output goes in RetJsonObject
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Json Failed - Was not able to deserialize the json string. Is it the right format? - '%s'"), *JsonString);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json Succeeded - '%s'"), *JsonFilePath);
	return RetJsonObject;
}

void UJsonFunctionLib::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSucssess, FString& OutInfoMessage)
{
	FString JsonString;

	// Try to convert json object to string. Output goes in JsonString
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		bOutSucssess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Json Failed - Was not able to serialize the json to string. Is the JsonObject valid?"));
		return;
	}

	// Try to write json string to file
	UFileIOFunctionLib::WriteStringToFile(JsonFilePath, JsonString, bOutSucssess, OutInfoMessage);
	if (!bOutSucssess)
	{
		return;
	}

	bOutSucssess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json Succeeded - '%s'"), *JsonFilePath);
}
