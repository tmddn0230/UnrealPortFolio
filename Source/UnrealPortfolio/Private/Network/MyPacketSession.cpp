// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/MyPacketSession.h"
// Protocol
#include "UPCore/Public/MyProtocol.h"
// Network
#include "Network/MyWorkerThread.h"
#include "Network/MyNetworkManager.h"

///////////////////////////////////////////////////////////
/////////////         TCP Network            /////////////
//////////////////////////////////////////////////////////

MyPacketSession::MyPacketSession(FSocket* sock) : Socket(sock)
{
}

MyPacketSession::~MyPacketSession()
{
	DisConnect();
}

void MyPacketSession::RunThread()
{
	RecvWorkerThread_T = MakeShared<RecvWorker_T>(Socket, AsShared());
	SendWorkerThread_T = MakeShared<SendWorker_T>(Socket, AsShared());
}

void MyPacketSession::DisConnect()
{
	if (RecvWorkerThread_T)
	{
		RecvWorkerThread_T->Destroy();
		RecvWorkerThread_T = nullptr;
	}

	if (SendWorkerThread_T)
	{
		SendWorkerThread_T->Destroy();
		SendWorkerThread_T = nullptr;
	}
}

void MyPacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		auto* GameInstance = GWorld->GetGameInstance()->GetSubsystem<UMyNetworkManager>();

		if (GameInstance)
		{
			stHeader header;

			memcpy(&header, Packet.GetData(), sizeof(stHeader));

			int32 protocol = header.nID;

			GameInstance->Parse(protocol, Packet);
		}
	}
}




SendBuffer_T::SendBuffer_T(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer_T::~SendBuffer_T()
{
}

void SendBuffer_T::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer_T::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}



///////////////////////////////////////////////////////////
/////////////         UDP Network            /////////////
//////////////////////////////////////////////////////////

MyPacketSession_U::MyPacketSession_U(FSocket* sock, TSharedPtr<FInternetAddr> InternetAddr_recv, TSharedPtr<FInternetAddr> InternetAddr_send)
	: Socket_U(sock), InternetAddr_Recv_U(InternetAddr_recv), InternetAddr_Send_U(InternetAddr_send)
{
}

MyPacketSession_U::~MyPacketSession_U()
{
	DisConnect();
}

void MyPacketSession_U::RunThread()
{
	RecvWorkerThread_U = MakeShared<RecvWorker_U>(Socket_U, AsShared());
	SendWorkerThread_U = MakeShared<SendWorker_U>(Socket_U, AsShared());
}

void MyPacketSession_U::DisConnect()
{
	if (RecvWorkerThread_U)
	{
		RecvWorkerThread_U->Destroy();
		RecvWorkerThread_U = nullptr;
	}

	if (SendWorkerThread_U)
	{
		SendWorkerThread_U->Destroy();
		SendWorkerThread_U = nullptr;
	}
}

void MyPacketSession_U::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue_U.Dequeue(OUT Packet) == false)
			break;

		auto* GameInstance = GWorld->GetGameInstance()->GetSubsystem<UMyNetworkManager>();

		if (GameInstance)
		{
			GameInstance->RecvVoiceBuffer(Packet);
		}
	}
}




SendBuffer_U::SendBuffer_U(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer_U::~SendBuffer_U()
{
}

void SendBuffer_U::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer_U::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}