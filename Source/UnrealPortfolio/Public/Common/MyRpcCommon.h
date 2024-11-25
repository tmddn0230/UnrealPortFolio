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

/*
* 시나리오 정보 
*/
USTRUCT(BlueprintType)
struct UNREALPORTFOLIO_API FScenarioInfo
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ScenarioName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayerNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TimeLimit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Summary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Destination;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FileName;


	FScenarioInfo() :
		ScenarioName(""),
		PlayerNum(1),
		TimeLimit(60),
		Summary(""),
		Destination(""),
		FileName("")
	{
	}
};

USTRUCT(BlueprintType)
struct UNREALPORTFOLIO_API FTrainingRoleInfo
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString RoleName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  int32 RID;
	
	FTrainingRoleInfo()
	{
		RoleName = "";
		RID = 0;
	}

	
	FTrainingRoleInfo(FString InName, int32 InID)
	{
		RoleName = InName;
		RID = InID;
	}
};

USTRUCT(BlueprintType)
struct UNREALPORTFOLIO_API FTraineeInfo
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString TraineeName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString TraineeSN;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)  float Traineeheight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 UID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FTrainingRoleInfo RoleInfo;

	static TArray<FTraineeInfo> DefualtArray;


	FTraineeInfo() :TraineeName(TEXT("Trainee")), TraineeSN(TEXT("Server Number")) {}
	void Empty() {
		TraineeName = "";
		TraineeSN = "";
		RoleInfo.RoleName = "";
		RoleInfo.RID = 0;
		Traineeheight = 0.0f;
		UID = 0;
	}
	// Update 
	FTraineeInfo(const FString& InTraineeSN, const FString& InTraineeName, const FString& InUID, const float& Inheight, const FTrainingRoleInfo& InRole)
	{
		TraineeName = InTraineeName;
		TraineeSN = InTraineeSN;
		Traineeheight = Inheight;
		RoleInfo = InRole;
		if (FCString::IsNumeric(*InUID))
			UID = FCString::Atoi(*InUID);
		else
			UID = 0;
	}
};

USTRUCT(BlueprintType)
struct UNREALPORTFOLIO_API FDeviceInfo
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString DeviceName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DeviceIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Active;    // true : login , for Control Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Ready;  
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayerId; // player id in playerstate

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Assigned_Trainee; // true : Assigned_Trainee

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Assigned_Role;    // true : Assigned_Role


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTraineeInfo PlayerInfo;


	static FDeviceInfo Defualt;
	static TArray<FDeviceInfo> DefualtArray;

	FDeviceInfo() :
		DeviceName(""),
		DeviceIndex(0),
		Active(false),
		Ready(false),
		PlayerId(0)
	{}
	FDeviceInfo(const FDeviceInfo& InInfo) :FDeviceInfo() {
		DeviceName = InInfo.DeviceName;
		DeviceIndex = InInfo.DeviceIndex;
		Active = InInfo.Active;
		Ready = InInfo.Ready;
		PlayerId = InInfo.PlayerId;
		Assigned_Trainee = InInfo.Assigned_Trainee;
		Assigned_Role = InInfo.Assigned_Role;
		PlayerInfo = InInfo.PlayerInfo;
	}
	void Logout() {
		Active = false;
		Ready = false;
		PlayerId = 0;
		Assigned_Trainee = false;
		Assigned_Role = false;
		PlayerInfo = FTraineeInfo();
	}

	// 관찰용 Logout
	void Logout_Ob()
	{
		Active = false;
		PlayerId = 0;
		Assigned_Trainee = false;
		Assigned_Role = false;
		PlayerInfo = FTraineeInfo();
	}
	void Reset_Trainee()
	{

	}

	void Assign_Trainee(const FTraineeInfo& InPlayerInfo) {
		PlayerInfo = InPlayerInfo;
	}

	void Assign_Role(const FTrainingRoleInfo& InRoleInfo) {
		PlayerInfo.RoleInfo = InRoleInfo;
	}

	bool IsValid_TraineeInformation() { //``Check valid trainee information
		if (PlayerInfo.UID == 0)
			return false;
		return true;
	}

	FString Get_String() {
		return FString::Printf(TEXT("[%s] %s,%d"), *DeviceName, *PlayerInfo.TraineeName, PlayerInfo.UID);
	}

	bool bHasTrainee() {
		return (PlayerInfo.UID > 0) ? true : false;
	}

	bool bHasRole() {
		return (PlayerInfo.RoleInfo.RID > 0) ? true : false;
	}

};
