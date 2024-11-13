// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/MyNetworkManager.h"
// Socket
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
// UE
#include "Serialization/ArrayWriter.h"
// Common
#include "Common/MyConfig.h"
// Data 
#include "UPCore/Public/MyPacket.h"
#include "UPCore/Public/MyProtocol.h"
// Network
#include "Network/MyPacketSession.h"
#include "Network/MyWorkerThread.h"

DEFINE_LOG_CATEGORY(LogNetworkManager);

//////////////////////////////////////////////////////////////////////////////
///// TCP  //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void UMyNetworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// SET INI DATAS
	IpAddress_TCP = UMyConfig::TCPSocketIP;
	Port_TCP      = UMyConfig::TCPPort;
	IpAddress_UDP = UMyConfig::UDPSocketIP;
	SendPort_UDP  = UMyConfig::UDPPort;
}

void UMyNetworkManager::Deinitialize()
{
	Super::Deinitialize();
	// TODO
}

void UMyNetworkManager::Connect()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress_TCP, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port_TCP);

	UE_LOG(LogNetworkManager, Log, TEXT("Connecting To Server"));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		UE_LOG(LogNetworkManager, Log, TEXT("Connection Success"));

		// Session
		GetWorld()->GetTimerManager().SetTimer(ReceiveTimerHandle_T, this, &UMyNetworkManager::HandleRecvPackets, 0.01f, true);
		GameServerPacketSession_T = MakeShared<MyPacketSession>(Socket);
		GameServerPacketSession_T->RunThread();
	}
	else
	{
		UE_LOG(LogNetworkManager, Log, TEXT("Connection Failed"));
	}
}

void UMyNetworkManager::Disconnect()
{
	Socket->Close();
}

void UMyNetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerPacketSession_T == nullptr)
		return;

	GameServerPacketSession_T->HandleRecvPackets();
}

void UMyNetworkManager::Parse(int32 protocol, TArray<uint8> packet)
{
	switch (protocol)
	{
	case prConnectAck:	RecvConnectAck(packet);	 break;
	case prLoginAck:    RecvLoginAck(packet);    break;
	case prSetSCTAck:   RecvSetSCTAck(packet);   break;
	case prStartAck:    RecvStartAck(packet);    break;
	case prFinish:      RecvFinish(packet);      break;
	case prAccident:    RecvAccident(packet);    break;
	case prDayNight:    RecvDayNight(packet);    break;
	case prRestartSame: RecvRestart(packet);     break;
	}
}

void UMyNetworkManager::SendPacket(stHeader* packetData)
{
	if (packetData)
	{
		int32 packetSize = packetData->nSize;

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBuffer_TRef sendBuffer = MakeShared<SendBuffer_T>(packetSize);
#else
		SendBuffer_TRef sendBuffer = nake_shared<SendBuffer_T>(packetSize);
#endif
		packetData->nSize = packetSize;
		sendBuffer->CopyData(packetData, packetSize);
		sendBuffer->Close(packetSize);

		if (GameServerPacketSession_T) {
			GameServerPacketSession_T->SendPacketQueue.Enqueue(sendBuffer);
		}
	}
}

void UMyNetworkManager::SendPacket_Login(FString playerObjectID, enRole Role)
{
	stLoginReq packetStruct;
	packetStruct.grade = Role;
	if (Role == enRole::ROLE_USER)
	{
		char* CharArray = TCHAR_TO_ANSI(*playerObjectID);
		memcpy(packetStruct.objID, CharArray, playerObjectID.Len());
	}

	SendPacket(&packetStruct);
}

void UMyNetworkManager::SendPacket_SetScenario(FString SCTFileName)
{
	stSetSCTReq packetStruct;

	wchar_t* CharArray = TCHAR_TO_WCHAR(*SCTFileName);
	memcpy(packetStruct.filename, CharArray, SCTFileName.Len() * 2);

	SendPacket(&packetStruct);
}

void UMyNetworkManager::SendPacket_TrainingStart(int32 TID)
{
	stStartReq packetStruct;
	packetStruct.TID = TID;

	SendPacket(&packetStruct);
}

void UMyNetworkManager::SendPacket_TrainingEnd(int32 TID)
{
	stFinish packetStruct;
	packetStruct.TID = TID;

	SendPacket(&packetStruct);
}

void UMyNetworkManager::SendPacket_PutAccident(FString AccidentName)
{
	if (AccidentName.Len() > 64)
		return;

	wchar_t* charArray = TCHAR_TO_WCHAR(*AccidentName);

	stAccident packetStruct;
	FMemory::Memcpy(packetStruct.AccidentName, charArray, AccidentName.Len() * 2);

	SendPacket(&packetStruct);
}

void UMyNetworkManager::SendPacket_DayNight(bool isNight)
{
	stDayNight packetStruct;
	packetStruct.IsNight = isNight;

	SendPacket(&packetStruct);
}

void UMyNetworkManager::SendPacket_RestartSame()
{
	stRestartSame packetStruct;

	SendPacket(&packetStruct);
}


/// recv
void UMyNetworkManager::RecvConnectAck(TArray<uint8> packet)
{
	stConnectAck ConnectAck;
	memcpy(&ConnectAck, packet.GetData(), sizeof(stConnectAck));

	RecvPort_UDP = ConnectAck.UDPPort;

	// if UsingVoice value is false, don't work udp client
	if (!UMyConfig::UsingVoice) return;

	//if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	//{
	//	ERt_PlayType myType = rtmgr->Get_Play_Type();
	//
	//	if (myType == ERt_PlayType::E_Trainee)
	//	{
	//		// UDP Start, only Client
	//		Start_UDP();
	//	}
	//}
}

void UMyNetworkManager::RecvLoginAck(TArray<uint8> packet)
{
	stLoginAck LoginAck;
	memcpy(&LoginAck, packet.GetData(), sizeof(LoginAck));

	switch (LoginAck.nResult)
	{
	case enErrorCode::LOGIN_SUCCESS:
		UE_LOG(LogNetworkManager, Log, TEXT("Login Success !!")) break;
	case enErrorCode::FAIL:
		UE_LOG(LogNetworkManager, Log, TEXT("Login Fail !!"))break;
	case enErrorCode::ERROR_DUP_ACCOUNT:
		UE_LOG(LogNetworkManager, Log, TEXT("Duplicate Account."))break;
	case enErrorCode::ERROR_ACCOUNT:
		UE_LOG(LogNetworkManager, Log, TEXT("No Account in Server."))break;
	case enErrorCode::ERROR_PASSWORD:
		UE_LOG(LogNetworkManager, Log, TEXT("Wrong Password."))break;
	default:
		break;
	}
}

void UMyNetworkManager::RecvSetSCTAck(TArray<uint8> packet)
{
}

void UMyNetworkManager::RecvStartAck(TArray<uint8> packet)
{
	stStartAck ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	/*
	if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		ERt_PlayType myType = rtmgr->Get_Play_Type();
		if (auto* tmgr = Get_TrainingManager())
		{
			// TID InValid
			if (ack.TID == 0) {
				UE_LOG(LogTemp, Warning, TEXT(" TID is not Invalid. "));
				return;
			}
			// SetTID
			tmgr->SetTID(ack.TID);

			if (myType == ERt_PlayType::E_Control)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Control : Training Start !! "));
				tmgr->SetControlFlow(EControlFlow::CONTROL_PROCESSINGTRAINING);

				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					pc->GetGameScenarioController()->SetGamePlay(true);
				}
			}
			else if (myType == ERt_PlayType::E_Trainee)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Trainee : Training Start !! "));
				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					pc->StartTraining(ack.TID);
				}

				// Find Voice Manager
				AActor* FoundActor;
				FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ARtVoiceManager::StaticClass());

				VM = Cast<ARtVoiceManager>(FoundActor);

				if (VM) {
					if (URtConfig::UsingVoice)
					{
						VM->Init();
						VM->SetNetManager(this);
						VM->StartCapture(); // Voice Capture
					}
				}
			}
			else if (myType == ERt_PlayType::E_Observer)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Observer : Training Start !! "));
				tmgr->SetControlFlow(EControlFlow::CONTROL_PROCESSINGTRAINING);
				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					pc->GetGameScenarioController()->SetGamePlay(true);
				}
			}
		}
	}
	*/
}

void UMyNetworkManager::RecvFinish(TArray<uint8> packet)
{
	stFinish ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));

	/*
	if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		ERt_PlayType myType = rtmgr->Get_Play_Type();
		if (auto* tmgr = Get_TrainingManager())
		{
			// TID InValid
			if (ack.TID == 0) {
				UE_LOG(LogTemp, Warning, TEXT(" TID is not Invalid. "));
				return;
			}
			// SetTID
			tmgr->SetTID(ack.TID);

			if (myType == ERt_PlayType::E_Control)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Control : Training Finish !! "));
				tmgr->SetControlFlow(EControlFlow::CONTROL_RESULT);
				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					// 관제 Player Controller에 접근
					//pc->Server_Stop_Record();
					pc->GetGameScenarioController()->SetGamePlay(false);
				}
			}
			else if (myType == ERt_PlayType::E_Trainee)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Trainee : Training Finish !! "));
				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					// 클라 Player Controller에 접근
					pc->GetGameScenarioController()->SetGamePlay(false);

					FRtDeviceInfo outInfo;
					pc->Get_DeviceInfo(outInfo);

					if (VM) {
						if (URtConfig::UsingVoice)
						{
							VM->ExportCapture(ack.TID, outInfo.DeviceIndex, outInfo.Get_String());
						}
					}
				}
			}
		}
	}
	*/
}

void UMyNetworkManager::RecvAccident(TArray<uint8> packet)
{
	stAccident ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	FString AccidentName_str = UTF8_TO_TCHAR(ack.AccidentName);
}

void UMyNetworkManager::RecvDayNight(TArray<uint8> packet)
{
	stDayNight ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	//if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
	//	ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
	//	ARtGameSceneManager* gameSceneManager = Cast<ARtGameSceneManager>(pc->GetGameScenarioController()->GetSceneManagerRef());
	//	if (gameSceneManager) {
	//		gameSceneManager->SetDayNight(!ack.IsNight);
	//	}
	//	else {
	//		UE_LOG(LogTemp, Warning, TEXT(" SceneManager is Not Found! "));
	//	}
	//}
}

void UMyNetworkManager::RecvRestart(TArray<uint8> packet)
{
	//if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
	//	ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
	//	pc->GetGameScenarioController()->SetGamePlay(false);
	//	pc->GetGameScenarioController()->InitScenario();
	//
	//	ERt_PlayType myType = gmgr->Get_Play_Type();
	//	if (myType == ERt_PlayType::E_Control) {
	//		if (auto* tmgr = Get_TrainingManager()) {
	//			tmgr->SetControlFlow(EControlFlow::CONTROL_WAITINGTRAINING);
	//		}
	//	}
	//	else if (myType == ERt_PlayType::E_Trainee) {
	//		if (VM) {
	//			if (URtConfig::UsingVoice)
	//			{
	//				VM->RestartCapture();
	//			}
	//		}
	//	}
	//}
}



//////////////////////////////////////////////////////////////////////////////
///// UDP  //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void UMyNetworkManager::Start_UDP()
{
}

void UMyNetworkManager::End_UDP()
{
}

void UMyNetworkManager::HandleRecvPackets_UDP()
{
}

void UMyNetworkManager::SendPacket_UDP(const TArray<uint8>& Data, int CompressedSize)
{
}

void UMyNetworkManager::RecvVoiceBuffer(TArray<uint8> packet)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, packet]()
	{
			stVoiceHeader Header;
			memcpy(&Header, packet.GetData(), VHEADSIZE);

			int CompressedSize = Header.nCompressedSize;
			int TotalPacketSize = Header.nTotalPacketSize;
			int PayLoadSize = TotalPacketSize - VHEADSIZE;

			if (TotalPacketSize == packet.Num()) // 홀펀칭 거르고 이상한 패킷 거름
			{
				TArray<uint8> VoiceData;
				VoiceData.SetNumUninitialized(PayLoadSize);
				memcpy(VoiceData.GetData(), packet.GetData() + VHEADSIZE, PayLoadSize);

				// 메인 스레드에서 데이터 처리
				AsyncTask(ENamedThreads::GameThread, [this, VoiceData, CompressedSize]()
				{
					//if (VM) {
					//	VM->DecodedAndRun(VoiceData, CompressedSize);
					//	UE_LOG(LogTemp, Warning, TEXT(" Recieved Buffer - Size %d"), VoiceData.Num());
					//}
				});
			}
	});
}
