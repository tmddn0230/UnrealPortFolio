// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonFunctionLib.generated.h"

/**
 * 
 */
UCLASS()
class UPCORE_API UJsonFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Open a json file, read its content and convert it to a JsonObject
	*
	* @param JsonFilePath     the path of your json file : "C :/Temp/MyJson.json"
	* @param bOutSuccess      If the action was a success or not
	* @param OutInfoMessage   More Information about the action's result
	*
	* @return The JsonObject content of your json file

	*/
	static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Convert your JsonObject to a string, create a json file and dump the string in it
	 *
	 * @param JsonFilePath      the path of your json file : "C:/Temp/MyJson.json
	 * @param JsonObject        the JsonObject to want to write in the file
	 * @param bOutSuccess       If the action was a success or not
	 * @param OutInfoMessage    More Information about the action's result
	 */
	static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSucssess, FString& OutInfoMessage);
	



	template<typename T>
	static T ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage) {

		// Try to read generic json object from file
		TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, bOutSuccess, OutInfoMessage);


		if (nullptr != JsonObject)
		{
			T TemplateStruct;

			// Try to convert generic json object to the desired structure. Output goes in RetRtTemplateStruct
			if (!FJsonObjectConverter::JsonObjectToUStruct<T>(JsonObject.ToSharedRef(), &TemplateStruct))
			{
				bOutSuccess = false;
				OutInfoMessage = FString::Printf(TEXT("Read Struct Json Failed - Was not able to convert the json object to your desired structure. Is it the rigth format / struct? - '%s'"), *JsonFilePath);
				return T::StaticStruct();
			}

			bOutSuccess = true;
			OutInfoMessage = FString::Printf(TEXT("Read Struct Json Succeeded - '%s'"), *JsonFilePath);
			return TemplateStruct;
		}

		return T::StaticStruct();
	}

	template<typename T>
	void WriteStructToJsonFile(FString JsonFilePath, T Struct, bool& bOutSuccess, FString& OutInfoMessage)
	{
		// Try to convert struct to generic json object
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
		if (JsonObject == nullptr)
		{
			bOutSuccess = false;
			OutInfoMessage = FString::Printf(TEXT("Write Struct Json Failed - Was not able to convert the struct to a json object. This shouldn't really happen. "));
			return;
		}

		// Try to write json to file
		WriteJson(JsonFilePath, JsonObject, bOutSuccess, OutInfoMessage);
	}

};
