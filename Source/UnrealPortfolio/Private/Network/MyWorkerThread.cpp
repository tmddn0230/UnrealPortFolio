// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/MyWorkerThread.h"
// Socket
#include "Sockets.h"
#include "Common/UdpSocketBuilder.h"
// Packet
#include "UPCore/Public/MyPacket.h"

DEFINE_LOG_CATEGORY(LogNetworkThread);

RecvWorker_T::RecvWorker_T(FSocket* sock, TSharedPtr<MyPacketSession> session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvWorker_T::~RecvWorker_T()
{
	Destroy();
}

bool RecvWorker_T::Init()
{
	if (Thread) {
		UE_LOG(LogNetworkThread, Log, TEXT("Recv Thread Init"));
		return true;
	}
	else
	{
		UE_LOG(LogNetworkThread, Log, TEXT("Failed Thread Init"));
		return false;
	}
}

uint32 RecvWorker_T::Run()
{
	while (bRun)
	{
		TArray<uint8> Packet;
		if (ReceivePacket(OUT Packet))
		{
			if (TSharedPtr<MyPacketSession> pSession = Session.Pin())
			{
				pSession->RecvPacketQueue.Enqueue(Packet);
			}
		}
	}
	return 0;
}

void RecvWorker_T::Exit()
{
	bRun = false;
}

void RecvWorker_T::Destroy()
{
	bRun = false;
}

bool RecvWorker_T::ReceivePacket(TArray<uint8>& OutPacket)
{
	// 패킷 헤더 파싱
	const int32 HeaderSize = HEADSIZE;//sizeof( stHeader );
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	// ID, Size 추출
	stHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.nID, Header.nSize);
	}
	// 패킷 헤더 복사
	OutPacket = HeaderBuffer;

	// 패킷 내용 파싱
	TArray<uint8> PayloadBuffer;
	const int32 PayloadSize = Header.nSize - HeaderSize;
	if (PayloadSize == 0)
		return true;

	OutPacket.AddZeroed(PayloadSize);

	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;

	return false;
}

bool RecvWorker_T::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	if (!Socket) return false;

	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------
// SendWorker_TCP
//--------------------------------------------------------------------------------------------------------------------------

SendWorker_T::SendWorker_T(FSocket* sock, TSharedPtr<MyPacketSession> session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker_T::~SendWorker_T()
{
	Destroy();
}

bool SendWorker_T::Init()
{
	if (Thread) {
		UE_LOG(LogNetworkThread, Log, TEXT("Send Thread Init"));
		return true;
	}
	else
	{
		UE_LOG(LogNetworkThread, Log, TEXT("Failed Thread Init"));
		return false;
	}
}

uint32 SendWorker_T::Run()
{
	while (bRun)
	{
		SendBuffer_TRef SendBuffer;

		if (MyPacketSessionRef pSession = Session.Pin())
		{
			if (pSession->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}
	}

	return 0;
}

void SendWorker_T::Exit()
{
	bRun = false;
}

bool SendWorker_T::SendPacket(SendBuffer_TRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker_T::Destroy()
{
	bRun = false;
}

bool SendWorker_T::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}



///////////////////////////////////////////////////////////
/////////////         UDP Network            /////////////
//////////////////////////////////////////////////////////

RecvWorker_U::RecvWorker_U(FSocket* sock, TSharedPtr<MyPacketSession_U> session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThreadForUDP"));
}

RecvWorker_U::~RecvWorker_U()
{
	Destroy();
}

bool RecvWorker_U::Init()
{
	if (Thread) {
		UE_LOG(LogNetworkThread, Log, TEXT("Recv Thread Init"));
		return true;
	}
	else
	{
		UE_LOG(LogNetworkThread, Log, TEXT("Failed Thread Init"));
		return false;
	}
}

uint32 RecvWorker_U::Run()
{
	while (bRun)
	{
		TArray<uint8> Packet;
		if (ReceivePacket(OUT Packet))
		{
			if (Packet.IsEmpty()) continue;

			if (MyPacketSession_URef pSession = Session.Pin())
			{
				pSession->RecvPacketQueue_U.Enqueue(Packet);
			}
		}
	}
	return 0;
}

void RecvWorker_U::Exit()
{
	bRun = false;
}

void RecvWorker_U::Destroy()
{
	bRun = false;
}

bool RecvWorker_U::ReceivePacket(TArray<uint8>& OutPacket)
{
	if (!Socket) return false;
	uint32 ReceivedDataSize;
	TArray<uint8> ReceivedData;

	while (Socket->HasPendingData(ReceivedDataSize))
	{
		ReceivedData.SetNumUninitialized(ReceivedDataSize);
		// maybe need empty addr
		int32 BytesRead = 0;
		Socket->RecvFrom(ReceivedData.GetData(), ReceivedDataSize, OUT BytesRead, *Session.Pin()->InternetAddr_Recv_U);
		OutPacket = ReceivedData;
	}
	FPlatformProcess::Sleep(0.001f); // 1ms
	return true;
}


//--------------------------------------------------------------------------------------------------------------------------
// SendWorker_UDP
//--------------------------------------------------------------------------------------------------------------------------

SendWorker_U::SendWorker_U(FSocket* sock, TSharedPtr<MyPacketSession_U> session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThreadForUDP"));
}

SendWorker_U::~SendWorker_U()
{
	Destroy();
}

bool SendWorker_U::Init()
{
	if (Thread) {
		UE_LOG(LogNetworkThread, Log, TEXT("Send Thread Init"));
		return true;
	}
	else
	{
		UE_LOG(LogNetworkThread, Log, TEXT("Failed Thread Init"));
		return false;
	}
}

uint32 SendWorker_U::Run()
{
	while (bRun)
	{
		SendBuffer_URef SendBuffer;

		if (MyPacketSession_URef pSession = Session.Pin())
		{
			if (pSession->SendPacketQueue_U.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}
	}

	return 0;
}

bool SendWorker_U::SendPacket(SendBuffer_URef SendBuffer)
{
	if (Socket == nullptr) {
		UE_LOG(LogNetworkThread, Log, TEXT("InValid Socket"));
		return false;
	}
	uint8* data = SendBuffer->Buffer();
	int32 size = SendBuffer->WriteSize();
	int32 sent = 0;

	if (size <= 0) return true;
	// Server's Recv Port
	Socket->SendTo(data, size, sent, *Session.Pin()->InternetAddr_Send_U);
	return true;
}

void SendWorker_U::Exit()
{
	bRun = false;
}

void SendWorker_U::Destroy()
{
	bRun = false;
}
