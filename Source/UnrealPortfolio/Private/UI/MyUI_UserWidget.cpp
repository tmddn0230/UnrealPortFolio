// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUI_UserWidget.h"
// Common
#include "Common/MyCommon.h"
// Framework
#include "Framework/MyGameMode.h"
#include "Framework/MyGameState.h"
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"
// Manager
#include "Manager/MyGameManager.h"
// Network
#include "Network/MyHttpManager.h"


UMyGameManager* UMyUI_UserWidget::GetGameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<UMyGameManager>();

	return nullptr;
}

AMyPlayerController* UMyUI_UserWidget::Get_PlayerController()
{
	return Cast<AMyPlayerController>(GetOwningPlayer());
}

AMyGameState* UMyUI_UserWidget::Get_GameState()
{
	return GetWorld() != NULL ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
}

AMyGameMode* UMyUI_UserWidget::Get_GameMode()
{
	UWorld* world = GetWorld();
	check(world);
	return Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
}

AMyGameHUD* UMyUI_UserWidget::Get_HUD()
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Get_HUD();
	}
	return nullptr;
}

UMyHttpManager* UMyUI_UserWidget::GetHttpManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<UMyHttpManager>();

	return nullptr;
}

bool UMyUI_UserWidget::IsControl_PlayType()
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Get_PlayType() == EUP_PlayType::E_Control;
	}
	return false;
}

bool UMyUI_UserWidget::IsLocalPlayer()
{
	if (auto* pc = Get_PlayerController()) {
		if (pc->IsLocalController())
			return true;
	}
	return false;
}

bool UMyUI_UserWidget::IsLocalConllerPlayType()
{
	if (auto* pc = Get_PlayerController()) {
		if (pc->IsLocalController())
			return pc->Get_PlayType() == EUP_PlayType::E_Control;
	}
	return false;
}

