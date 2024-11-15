// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Exe Type 
UENUM(BlueprintType)
enum class EUP_PlayerMode : uint8
{
	NONE,
	PC        = 10,
	HMD       = 20,
	Optitrack = 30,
	Mocopi    = 40,
	Alt       = 50
};

enum class EUP_GameMode
{
	E_None,
	E_Lobby,
	E_SinglePlay,
	E_MultiPlay
};

enum class EUP_PlayState
{
	E_NoneState,
	E_Allocating,   // 관제에서 훈련생 할당과정 까지
	E_Ready,        // 관제에서 할당 후 클라이언트 들이 준비하는 과정 (켈리브레이션)
	E_Progress,     // 관제에서 훈련 시작 
	E_Result        // 종료
};

UENUM(BlueprintType)
enum class EUP_PlayType : uint8
{
	E_None	    UMETA(DisplayName = "None"),
	E_Single    UMETA(DisplayName = "Single"),
	E_Server	UMETA(DisplayName = "Server"),
	E_Client	UMETA(DisplayName = "Client"),
	E_Control   UMETA(DisplayName = "Controller"),
	E_Observer  UMETA(DisplayName = "Observer"),
	E_Replay    UMETA(DisplayName = "Replay"),
	E_Max       UMETA(DisplayName = "Max")
};

/*
* 선택을 해야하는 UI 인터렉션에서 사용 
*/
UENUM(BlueprintType)
enum class EUP_MessageResult : uint8
{
	/** The "yes" button was pressed */
	Confirmed,
	/** The "no" button was pressed */
	Declined,
	/** The "ignore/cancel" button was pressed */
	Cancelled,
	/** The dialog was explicitly killed (no user input) */
	Killed,
	Unknown UMETA(Hidden)
};


////////////////////////////////////////////////
//////             Optitrack             //////
//////////////////////////////////////////////
UENUM()
enum class EOptitrackMarker
{
	Head,
	Pelvis,
	LHand,
	RHand,
	LFoot,
	RFoot,
	Weapon,
	Max
};

UENUM()
enum class EOptitrackMarkerID
{
	Pelvis = 0,
	PalmLeft = 1,
	PalmRight = 2,
	FootLeft = 3,
	FootRight = 4,
	Head = 5,
};










class UNREALPORTFOLIO_API MyCommon
{
public:
	MyCommon();
	~MyCommon();
};
