// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyModeDefinition.h"

FName UMyModeDefinition::Get_UI(EUP_PlayType InPlayType)
{
	switch (InPlayType)
	{
	case EUP_PlayType::E_Single:
		return SingleMode.UI_Name;
	case EUP_PlayType::E_Client:
		return ClientMode.UI_Name;
	case EUP_PlayType::E_Replay:
		return ReplayMode.UI_Name;
	case EUP_PlayType::E_Control:
		return ControlMode.UI_Name;
	case EUP_PlayType::E_Server:
		return ServerMode.UI_Name;
	case EUP_PlayType::E_Observer:
		return ObserveMode.UI_Name;
	default:
		return NAME_None;
	}
}

const FMyModeInfo& UMyModeDefinition::Get_Mode(EUP_PlayType InPlayType)
{
	switch (InPlayType)
	{
	case EUP_PlayType::E_Single:
		return SingleMode;
	case EUP_PlayType::E_Client:
		return ClientMode;
	case EUP_PlayType::E_Replay:
		return ReplayMode;
	case EUP_PlayType::E_Control:
		return ControlMode;
	case EUP_PlayType::E_Server:
		return ServerMode;
	case EUP_PlayType::E_Observer:
		return ObserveMode;
	default:
		return DefaultMode;
	}
}
