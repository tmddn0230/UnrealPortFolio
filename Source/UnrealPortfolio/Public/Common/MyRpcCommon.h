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
*  ������Ʈ ���� ���� ( IP, ���̸�, Ʈ��Ŀ ���� �� )
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
*  �Ʒ����� �α��� ���� 
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