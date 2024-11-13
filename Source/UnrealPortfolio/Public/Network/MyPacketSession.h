// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;
class SendBuffer_T;
class SendBuffer_U;

#define USING_SHARED_PTR(name)	using name##Ref = TSharedPtr<class name>;

// TShared 중복
USING_SHARED_PTR(Session);
USING_SHARED_PTR(MyPacketSession);
USING_SHARED_PTR(MyPacketSession_U);
USING_SHARED_PTR(SendBuffer_T);
USING_SHARED_PTR(SendBuffer_U);


///////////////////////////////////////////////////////////
/////////////         TCP Network            /////////////
//////////////////////////////////////////////////////////
class UNREALPORTFOLIO_API MyPacketSession : public TSharedFromThis< MyPacketSession >	// TShared 새로 생성하는 실수 방지
{
public:
	FSocket* Socket;
	TSharedPtr< class RecvWorker_T > RecvWorkerThread_T;
	TSharedPtr< class SendWorker_T > SendWorkerThread_T;

	// GameThread와 NetworkThread가 데이터 주고 받는 공용 큐.
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBuffer_TRef> SendPacketQueue;


public:
	MyPacketSession(FSocket* sock);
	~MyPacketSession();

	void RunThread();
	void DisConnect();


	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

};

class SendBuffer_T : public TSharedFromThis<SendBuffer_T>
{
public:
	SendBuffer_T(int32 bufferSize);
	~SendBuffer_T();


	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);


private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};


///////////////////////////////////////////////////////////
/////////////         UDP Network            /////////////
//////////////////////////////////////////////////////////
class UNREALPORTFOLIO_API MyPacketSession_U : public TSharedFromThis<MyPacketSession_U>
{
public:
	FSocket* Socket_U;
	TSharedPtr<FInternetAddr> InternetAddr_Recv_U;
	TSharedPtr<FInternetAddr> InternetAddr_Send_U;
	TSharedPtr< class RecvWorker_U > RecvWorkerThread_U;
	TSharedPtr< class SendWorker_U > SendWorkerThread_U;

	TQueue<TArray<uint8>> RecvPacketQueue_U;
	TQueue<SendBuffer_URef> SendPacketQueue_U;

public:
	MyPacketSession_U(FSocket* sock, TSharedPtr<FInternetAddr> InternetAddr_recv, TSharedPtr<FInternetAddr> InternetAddr_send);
	~MyPacketSession_U();

	void RunThread();
	void DisConnect();


	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

};


class SendBuffer_U : public TSharedFromThis<SendBuffer_U>
{
public:
	SendBuffer_U(int32 bufferSize);
	~SendBuffer_U();


	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);


private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};

