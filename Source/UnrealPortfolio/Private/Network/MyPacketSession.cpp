// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/MyPacketSession.h"

///////////////////////////////////////////////////////////
/////////////         TCP Network            /////////////
//////////////////////////////////////////////////////////

MyPacketSession::MyPacketSession(FSocket* sock) : Socket(sock)
{
}

MyPacketSession::~MyPacketSession()
{
}

void MyPacketSession::RunThread()
{
}

void MyPacketSession::DisConnect()
{
}

void MyPacketSession::HandleRecvPackets()
{
}




SendBuffer_T::SendBuffer_T(int32 bufferSize)
{
}

SendBuffer_T::~SendBuffer_T()
{
}

void SendBuffer_T::CopyData(void* data, int32 len)
{
}

void SendBuffer_T::Close(uint32 writeSize)
{
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
}

void MyPacketSession_U::RunThread()
{
}

void MyPacketSession_U::DisConnect()
{
}

void MyPacketSession_U::HandleRecvPackets()
{
}

SendBuffer_U::SendBuffer_U(int32 bufferSize)
{
}

SendBuffer_U::~SendBuffer_U()
{
}

void SendBuffer_U::CopyData(void* data, int32 len)
{
}

void SendBuffer_U::Close(uint32 writeSize)
{
}