#pragma once

#include "CoreMinimal.h"
#include "MyProtocol.h"

#pragma pack(1)
struct stConnectAck : public stHeader
{
	int32 UDPPort;
	int32 Index;

	stConnectAck()
	{
		UDPPort = 0;
		Index = 0;

		SetHeader( prConnectAck, sizeof( stConnectAck ) );
	};
};

struct stLoginReq : public stHeader
{
	int32 grade;
	int8 objID[ 64 ];

	stLoginReq()
	{
		grade = ROLE_USER;
		memset( objID, 0x00, sizeof( objID ) );
		SetHeader( prLoginReq, sizeof( stLoginReq ) );
	}
};

struct stLoginAck : public stHeader
{
	//1이 아니면 실패  예) -1 objID 가 SCT 그룹 리스트에 없음
	int32 nResult;	

	stLoginAck()
	{
		nResult = 0;
		
		SetHeader( prLoginAck, sizeof( stLoginAck ) );
	}
};


struct stSetSCTReq : public stHeader
{
	int16 filename[ 64 ];
	stSetSCTReq()
	{
		memset( filename, 0x00, sizeof( filename ) );
		SetHeader( prSetSCTReq, sizeof( stSetSCTReq ) );
	}
};

struct stReady : public stHeader
{
	int32 UID;
	bool IsReady;
	stReady()
	{
		UID = 0;
		IsReady = false;
		SetHeader(prReady, sizeof(stReady));
	}

};


struct stSetSCTAck : public stHeader
{
	int16 filename[ 64 ];
	stSetSCTAck()
	{
		memset( filename, 0x00, sizeof( filename ) );
		SetHeader( prSetSCTAck, sizeof( stSetSCTAck ) );
	}
};

struct stStartReq : public stHeader
{
	int32 TID;
	stStartReq()
	{
		TID = 0;
		SetHeader( prStartReq, sizeof( stStartReq ) );
	}
};
struct stStartAck : public stHeader
{
	int32 TID;
	stStartAck()
	{
		TID = 0;
		SetHeader( prStartAck, sizeof( stStartAck ) );
	}
};


struct stFinish : public stHeader
{
	int32 TID;
	stFinish()
	{
		TID = 0;
		SetHeader( prFinish, sizeof( stFinish ) );
	}
};


struct stAccident : public stHeader
{
	int16 AccidentName[64];
	stAccident() {
		memset(AccidentName, 0x00, sizeof(AccidentName));
		SetHeader(prAccident, sizeof(stAccident));
	}
};


struct stDayNight : public stHeader
{
	bool IsNight;
	stDayNight()
	{
		IsNight = false;
		SetHeader(prDayNight, sizeof(stDayNight));
	}
};

struct stRestartSame : public stHeader
{
	stRestartSame()
	{
		SetHeader(prRestartSame, sizeof(stRestartSame));
	}
};

struct stRestartAll : public stHeader
{
	stRestartAll()
	{
		SetHeader(prRestartAll, sizeof(stRestartAll));
	}
};

//TOOL--------------------------------------------------------------------------------

#pragma pack()
