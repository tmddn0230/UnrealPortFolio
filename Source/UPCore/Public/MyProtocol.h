#pragma once
#pragma pack(1)

#include "CoreMinimal.h"

#define GAME_ID 1818

enum enErrorCode
{
	ERROR_NODEVICE = -43,
	ERROR_NOTRAINEE = -42,
	ERROR_ALREADY_RESULT = -41, // There is Result of Training already.
	ERROR_NOTRAININGNUMBER = -40,
	ERROR_DUP_ROLE = -7, // Duplicate Role ID
	ERROR_DUP_TRAINEE = -6, // Duplicate Trainee (UID)
	ERROR_DUO_NUMBER = -5 , // Duplicate Army Number 
	ERROR_PASSWORD = -4,
	ERROR_ACCOUNT = -2,   // No Account
	ERROR_DUP_ACCOUNT = -1,
	FAIL = 0,
	LOGIN_SUCCESS = 1 
};


enum enRole
{
	ROLE_START = 0,
	ROLE_ADMIN,
	ROLE_TRANDER,
	ROLE_OBSERVER,
	ROLE_USER,
	ROLE_END
};

enum enProtocol
{
	PROTOCOL_START = 0,
					prConnectAck,
	prLoginReq,		prLoginAck,
	prSetSCTReq,	prSetSCTAck,
	prStartReq,		prStartAck,
	prFinish,       prReady,
	prAccident,
	prSctEvent,		prInteractKey,
	prTraineeAim,	prTraineeShot,
	prDayNight,     
	prRestartSame,  prRestartAll,
	PROTOCOL_END,
};


struct stHeader
{
	uint16 nID;
	uint16 nSize;
	uint16 nType;
	uint16 nCheckSum;

	stHeader()
	{
		nID = nSize = nType = nCheckSum = 0;
	};
	void SetHeader( int id, int len )
	{
		nType = GAME_ID;
		nID = id;
		nSize = len;
		nCheckSum = nType + nID + nSize;
	};

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1

	friend FArchive& operator<<( FArchive& Ar, stHeader& Header )
	{
		Ar << Header.nID;
		Ar << Header.nSize;
		Ar << Header.nType;
		Ar << Header.nCheckSum;
		return Ar;
	}

#endif
};


struct stVoiceHeader
{
	uint16 nCompressedSize;
	uint16 nTotalPacketSize;
	uint16 nType; // DEFINE
	uint16 nCheckSum;

	stVoiceHeader()
	{
		nCompressedSize = nTotalPacketSize = nType = nCheckSum = 0;
	};

	void SetHeader(int CompressedSize, int len)
	{
		nType = GAME_ID;
		nCompressedSize = CompressedSize;
		nTotalPacketSize = len;
		nCheckSum = nType + CompressedSize + len;
	};

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1

	friend FArchive& operator<<(FArchive& Ar, stVoiceHeader& Header)
	{
		Ar << Header.nCompressedSize;
		Ar << Header.nTotalPacketSize;
		Ar << Header.nType;
		Ar << Header.nCheckSum;
		return Ar;
	}
#endif
};


#define HEADSIZE  sizeof( stHeader )
#define VHEADSIZE sizeof( stVoiceHeader )


////////////////////////////////////////////////
//////             Http Web              //////
//////////////////////////////////////////////
UENUM()
enum class ELogProtocol
{
	// protocol, TID , UID
	LOG_START = 0,
	LOG_FIRE,                // 사격
	LOG_HIT,                 // 적중
	LOG_CHALLENGE,           // 수하
	LOG_SURRENDER,           // 투항명령
	LOG_BIND,                // 포박
	LOG_COMMANDMOVE,         // 이동지시
	LOG_BREIFSITUATION,      // 상황보고
	LOG_CALL,                // 증원요청
	LOG_EOD,			     // EOD 요청
	LOG_BADFIRE,             // 오인사격
	//-----------------------------------------------------

	LOG_TR_HMD,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}
	LOG_TR_GUN,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}

	LOG_DISTANCE,            // float 훈련 종료 시 클라 -> 웹로그 호출

	LOG_MOTION,             // tid uid motion( json )
	LOG_OBJECT,             // tid motion
	LOG_END
};

enum class EWebProtocolType
{
	webErrorServerCheck = -5,
	webErrorBlock = -4,
	WebError = -3,	//webError
	webErrorDup = -2,
	webErrorParse = -1,
	WEB_PROTOCOL_START = 0,
	//-----------------------------------------------------

	webGetSCTList,
	webGetMCTList,
	webGetWCTList,
	webGetTCTList,
	webGetFBXList,
	webGetPNGList,
	webGetWAVList,
	webGetFBXAnimList,  //8
	webGetEffList,
	webGetAllFileList, // 

	webLogin,  //webLogin  11 
	webGetMachineList,
	webGetUserList,

	webStart,            // 14
	webFinish,
	webEnterUser,
	webTrainingUserList,
	webTrainingDetail,
	//-----------------------------------------------------
	webGetMotionLog,   //19, TID, UID
	webGetVoiceList,   // TID, recv i
	webGetObjectLog,   // TID
	WEB_PROTOCOL_END,  //WEB_PROTOCOL_END
};






#pragma pack()
