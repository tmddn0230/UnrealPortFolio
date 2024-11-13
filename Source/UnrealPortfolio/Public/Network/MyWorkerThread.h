// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPacketSession.h"

// Log
UNREALPORTFOLIO_API DECLARE_LOG_CATEGORY_EXTERN(LogNetworkThread, Log, All);

class FSocket;

///////////////////////////////////////////////////////////
/////////////         TCP Network            /////////////
//////////////////////////////////////////////////////////

class UNREALPORTFOLIO_API RecvWorker_T : public FRunnable
{
protected:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< MyPacketSession > Session;

public:
	RecvWorker_T(FSocket* sock, TSharedPtr< MyPacketSession > session);
	~RecvWorker_T();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);
};


class UNREALPORTFOLIO_API SendWorker_T : public FRunnable
{
protected:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< MyPacketSession > Session;

public:
	SendWorker_T(FSocket* sock, TSharedPtr< MyPacketSession > session);
	~SendWorker_T();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(SendBuffer_TRef SendBuffer);

	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);


};


///////////////////////////////////////////////////////////
/////////////         UDP Network            /////////////
//////////////////////////////////////////////////////////

class UNREALPORTFOLIO_API RecvWorker_U : public FRunnable
{
public:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< MyPacketSession_U > Session;

public:
	RecvWorker_U(FSocket* sock, TSharedPtr< MyPacketSession_U > session);
	~RecvWorker_U();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
};


class UNREALPORTFOLIO_API SendWorker_U : public FRunnable
{
public:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< MyPacketSession_U > Session;

public:
	SendWorker_U(FSocket* sock, TSharedPtr< MyPacketSession_U > session);
	~SendWorker_U();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(SendBuffer_URef SendBuffer);

	void Destroy();
};


