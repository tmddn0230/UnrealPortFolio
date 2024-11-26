// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
// Common
#include "Common/MyCommon.h"
#include "Common/MyRpcCommon.h"
#include "MyPlayerController.generated.h"


class AMyGameMode;
class AMyGameHUD;
class AMyPlayerState;

// Widget
class UMyUI_PageBase;
class UMyUI_PrimaryLayout;

UCLASS()
class UNREALPORTFOLIO_API AMyPlayerController : public APlayerController
{
public:
	// 변동이 되었을 때 콜백
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChanged_DeviceInfo, const FDeviceInfo&, AMyPlayerState*);
	FOnChanged_DeviceInfo OnChanged_DeviceInfo_delegate;

	// 임의로 업데이트 해야할 때 
	DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdate_DeviceList, APlayerController*, const TArray<FDeviceInfo>&);
	FUpdate_DeviceList    Update_DeviceList_Delegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdate_TraineeList, APlayerController*, const TArray<FTraineeInfo>&);
	FUpdate_TraineeList   Update_TraineeList_Delegate;

public:
	AMyPlayerController();

	// Main Flow
	GENERATED_BODY()
	void Init_PlayType();

	// Replicate
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Get
	AMyGameMode*                        Get_GameMode(); // for Only Server
	AMyGameHUD*                         Get_HUD();
	AMyPlayerState*                     Get_PlayerState();
	EUP_PlayType                        Get_PlayType();
	TWeakObjectPtr<UMyUI_PrimaryLayout> Get_PrimaryLayout();

	// Set
	void Set_Spectator(bool val);
	void Set_DeviceInfo(const TArray<FDeviceInfo>& InInfo);
	void Set_TraineeInfo(const TArray<FTraineeInfo>& InInfo);


	// UI
	UMyUI_PageBase* Open_Page(const FName& InName);
	UMyUI_PageBase* Open_Popup(const FName& InName);
	bool            Close_Popup(UMyUI_PageBase* InPage);

	// DelegateBindFunc
	void Handle_Changed_DeviceInfo();
	void CallBeforeBindingToPlayerState();


	// Server
	UFUNCTION(Reliable, Server)
	void Server_Send_DeviceInfo(int32 InId);
	UFUNCTION(Reliable, Server)
	void Server_Send_TraineeInfo(int32 InId);

	// Client
	UFUNCTION(Reliable, Client)
	void Client_Update_DeviceInfo(const TArray<FDeviceInfo>& InInfo);
	UFUNCTION(Reliable, Client)
	void Client_Update_TraineeInfo(const TArray<FTraineeInfo>& InInfo);

};
