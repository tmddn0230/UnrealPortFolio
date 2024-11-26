// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
// UE
#include "Kismet/Gameplaystatics.h"
#include "AbilitySystemInterface.h"
// Common
#include "Common/MyCommon.h"
#include "Common/MyRpcCommon.h"
#include "MyPlayerState.generated.h"

class AMyPlayerController;



UCLASS()
class UNREALPORTFOLIO_API AMyPlayerState : public APlayerState// , public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// ���κ� ����
	UPROPERTY(Replicated)
	FString Login_PlayerName;
	UPROPERTY(ReplicatedUsing = OnRep_DeviceInfoChanged)
	FDeviceInfo DeviceInfo;
	UPROPERTY(Replicated)
	EUP_PlayType PlayType;

	
	DECLARE_MULTICAST_DELEGATE_OneParam(FUpdated, AMyPlayerState*);
	static FUpdated Updated_Delegate;

	// UE Basic Delegate
	FSimpleMulticastDelegate OnChanged_DeviceInfo_Delegate;
	FSimpleMulticastDelegate OnChanged_TraineeInfo_Delegate;

public:
	AMyPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Replicate
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Called by Controller when its PlayerState is initially replicated. */
	virtual void ClientInitialize(class AController* C) override;

	// OnRep
	UFUNCTION()	void OnRep_DeviceInfoChanged(const FDeviceInfo& InInfo);

	// Delegate
	/*
	* PlayerController ���� ȣ���Ͽ� PlayerController ���� State ������ ȣ���ϰ� ���� �Լ� ���ε�
	*/
	void CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate Delegate);

	// Costom Get Set
	const FDeviceInfo&   Get_PlayerInfo();
	AMyPlayerController* Get_PlayerController();

	void                 Set_LoginInfo(const FString& InOptions, bool InServer);

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;

};
