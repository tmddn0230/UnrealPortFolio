// Fill out your copyright notice in the Description page of Project Settings.

// Framework
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"
#include "Framework/MyGameMode.h"
#include "Framework/MyGameHUD.h"

AMyGameMode* AMyPlayerController::Get_GameMode()
{
	UWorld* World = GetWorld();
	check(World);
	return Cast<AMyGameMode>(World->GetAuthGameMode());
}

AMyGameHUD* AMyPlayerController::Get_HUD()
{
	auto* res = Cast<AMyGameHUD>(GetHUD());
	return res;
}

AMyPlayerState* AMyPlayerController::Get_PlayerState()
{
	return Cast<AMyPlayerState>(PlayerState);
}
