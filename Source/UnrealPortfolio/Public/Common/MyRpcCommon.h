// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/MyCommon.h"
#include "MyRpcCommon.generated.h"

namespace MyString_Option
{
	extern UNREALPORTFOLIO_API const FString PlayerType;
	extern UNREALPORTFOLIO_API const FString DeviceName;
	extern UNREALPORTFOLIO_API const FString DeviceId;
}


/*
*  프로젝트 기초 세팅 ( IP, 맵이름, 트래커 정보 등 )
*/
USTRUCT(BlueprintType)
struct UNREALPORTFOLIO_API FPlayInitSetting
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ServerIp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NationCode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayViewMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TrackerGroupId;

	bool bDefaultData;
	FName ServerIptoName;

	FPlayInitSetting() :
		ServerIp(""),
		NationCode(0),
		PlayViewMode(0),
		TrackerGroupId(0),
		bDefaultData(true)
	{
	}

	void Set_ServerId(const FString& InStr) {
		ServerIp = InStr;
		ServerIptoName = FName(ServerIp);
	}

};

/*
*  훈련자의 로그인 정보 
*/

struct FTraineeInfo_Login
{
	FString Options;
	FString Address;
	FString UniqueId;
	FString ErrorMessage;
	FString TimeStr;

	EUP_PlayType PlayerType;
	FString PlayerName;
	FString PlayerId;
	FString PlayerPassword;
	FString DeviceName;
	FString DeviceId;

	void Set_Options(const FString& option_p);
};