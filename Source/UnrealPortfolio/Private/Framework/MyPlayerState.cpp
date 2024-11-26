// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyPlayerState.h"
#include "Framework/MyPlayerController.h"

//For DOREPLITETIME
#include "Net/UnrealNetwork.h"

AMyPlayerState::FUpdated AMyPlayerState::Updated_Delegate;


AMyPlayerState::AMyPlayerState(const FObjectInitializer& ObjectInitializer)
	: APlayerState(ObjectInitializer)
{
	NetUpdateFrequency = 100.0f;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, PlayType);
	DOREPLIFETIME(AMyPlayerState, Login_PlayerName);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(AMyPlayerState, DeviceInfo, SharedParams);
}

void AMyPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
}

void AMyPlayerState::OnRep_DeviceInfoChanged(const FDeviceInfo& InInfo)
{
	if (OnChanged_DeviceInfo_Delegate.IsBound() == false) {
		// 혹시라도 바인딩 안되어 있으면 바인드
		if (auto* pc = Get_PlayerController())
			pc->CallBeforeBindingToPlayerState();
	}
	else {
		OnChanged_DeviceInfo_Delegate.Broadcast();
	}

	// 위젯에서 필요한 기능 콜백을 위한 델리게이트
	Updated_Delegate.Broadcast(this);
}

void AMyPlayerState::CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate Delegate)
{
	bool bAleardyBound = Delegate.GetUObject() != nullptr ?
		OnChanged_DeviceInfo_Delegate.IsBoundToObject(Delegate.GetUObject()) : false;
	if (!bAleardyBound) {
		OnChanged_DeviceInfo_Delegate.Add(Delegate);
	}
}

const FDeviceInfo& AMyPlayerState::Get_PlayerInfo()
{
	return DeviceInfo;
}

AMyPlayerController* AMyPlayerState::Get_PlayerController()
{
	return Cast<AMyPlayerController>(GetPlayerController());
}

void AMyPlayerState::Set_LoginInfo(const FString& InOptions, bool InServer)
{
	FString temp = UGameplayStatics::ParseOption(InOptions, MyString_Option::PlayerType);
	uint8 temp_interger = FCString::Atoi(*temp);

	PlayType = (EUP_PlayType)temp_interger;
	Login_PlayerName = UGameplayStatics::ParseOption(InOptions, MyString_Option::DeviceName);
}

void AMyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

