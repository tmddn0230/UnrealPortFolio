// Fill out your copyright notice in the Description page of Project Settings.

// Framework
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"
#include "Framework/MyGameMode.h"
#include "Framework/MyGameHUD.h"

void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

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

EUP_PlayType AMyPlayerController::Get_PlayType()
{
	if (auto* playState = Get_PlayerState()) {
		return playState->PlayType;
	}
	return EUP_PlayType::E_None;
}
