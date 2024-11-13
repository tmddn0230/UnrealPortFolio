// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Protocol
#include "UPCore/Public/MyProtocol.h"
#include "MyNetworkManager.generated.h"



// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogNetworkManager, Log, All);

class UMyConfig;
class MyPacketSession;
class MyPacketSession_U;

UCLASS()
class UNREALPORTFOLIO_API UMyNetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	class FSocket* Socket;
	class FSocket* UDPSocket;

	// TCP
	FString IpAddress_TCP;
	int16 Port_TCP;
	TSharedPtr<MyPacketSession> GameServerPacketSession_T;
	FTimerHandle ReceiveTimerHandle_T;

	// UDP
	FString IpAddress_UDP;
	int16 SendPort_UDP;      // From MyConfig
	int16 RecvPort_UDP;      // From ConnectAck
	TSharedPtr<FInternetAddr> InternetAddr_UDP_Send;
	TSharedPtr<FInternetAddr> InternetAddr_UDP_Recv;
	TSharedPtr<MyPacketSession_U> GameServerPacketSession_U;
	FTimerHandle ReceiveTimerHandle_U;

public:
	// TCP
	void Connect();
	void Disconnect();
    void HandleRecvPackets();
	void Parse(int32 protocol, TArray<uint8> packet);

	// Basic Send
	void SendPacket               (stHeader* packetData);
	// Custom Send
	void SendPacket_Login         (FString playerObjectID, enRole Role);
	void SendPacket_SetScenario   (FString SCTFileName);
	void SendPacket_TrainingStart (int32 TID);
	void SendPacket_TrainingEnd   (int32 TID);
	void SendPacket_PutAccident   (FString AccidentName);
	void SendPacket_DayNight      (bool isNight);
	void SendPacket_RestartSame   ();

	// Recv
	void RecvConnectAck           (TArray<uint8> packet);
	void RecvLoginAck             (TArray<uint8> packet);
	void RecvSetSCTAck            (TArray<uint8> packet);
	void RecvStartAck             (TArray<uint8> packet);
	void RecvFinish               (TArray<uint8> packet);
	void RecvAccident             (TArray<uint8> packet);
	void RecvDayNight             (TArray<uint8> packet);
	void RecvRestart              (TArray<uint8> packet);

	// UDP
	void Start_UDP();
	void End_UDP();
	void HandleRecvPackets_UDP();

	// Send
	void SendPacket_UDP           (const TArray<uint8>& Data, int CompressedSize);

	//Recv
	void RecvVoiceBuffer          (TArray<uint8> packet);
};
