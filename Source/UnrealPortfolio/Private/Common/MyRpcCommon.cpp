// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MyRpcCommon.h"
// UE
#include "Kismet/Gameplaystatics.h"

namespace MyString_Option
{
	const FString PlayerType     = FString(L"PlayerType");
	const FString PlayerName     = FString(L"PlayerName");
	const FString PlayerId       = FString(L"PlayerId");
	const FString PlayerPassword = FString(L"PlayerPassword");
	const FString DeviceName     = FString(L"DeviceName");
	const FString DeviceId       = FString(L"DeviceId");
}

void FTraineeInfo_Login::Set_Options(const FString& option_p)
{
	Options = option_p;

	FString temp = UGameplayStatics::ParseOption(option_p, MyString_Option::PlayerType);
	uint8 temp_interger = FCString::Atoi(*temp);

	if (temp_interger >= (uint8)(EUP_PlayType::E_Max)) {
		PlayerType = EUP_PlayType::E_None;
	}
	else {
		PlayerType = (EUP_PlayType)temp_interger;
	}

	PlayerName     = UGameplayStatics::ParseOption(option_p, MyString_Option::PlayerName);
	PlayerId       = UGameplayStatics::ParseOption(option_p, MyString_Option::PlayerId);
	PlayerPassword = UGameplayStatics::ParseOption(option_p, MyString_Option::PlayerPassword);
	DeviceName     = UGameplayStatics::ParseOption(option_p, MyString_Option::DeviceName);
	DeviceId       = UGameplayStatics::ParseOption(option_p, MyString_Option::DeviceId);
}
