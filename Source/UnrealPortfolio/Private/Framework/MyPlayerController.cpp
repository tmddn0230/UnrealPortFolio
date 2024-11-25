// Fill out your copyright notice in the Description page of Project Settings.

// Framework
#include "Framework/MyPlayerController.h"
#include "Framework/MyPlayerState.h"
#include "Framework/MyGameMode.h"
#include "Framework/MyGameHUD.h"

// Widget
#include "UI/MyUI_PrimaryLayout.h"
#include "UI/MyUI_PageBase.h"

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

TWeakObjectPtr<UMyUI_PrimaryLayout> AMyPlayerController::Get_PrimaryLayout()
{
	// TODO 

	return TWeakObjectPtr<UMyUI_PrimaryLayout>();
}

UMyUI_PageBase* AMyPlayerController::Open_Page(const FName& InName)
{
	if (auto* hud = Get_HUD()) {
		return hud->Open_Page(InName, false);
	}
	else {
		TWeakObjectPtr<UMyUI_PrimaryLayout> primary_layout = Get_PrimaryLayout();
		if (primary_layout.IsValid()) {
			primary_layout.Get()->Open_Page(InName, false, this, nullptr);
		}
	}
	return nullptr;
}

UMyUI_PageBase* AMyPlayerController::Open_Popup(const FName& InName)
{
	if (auto* hud = Get_HUD()) {
		return hud->Open_Page(InName, true);
	}
	else {
		TWeakObjectPtr<UMyUI_PrimaryLayout> primary_layout = Get_PrimaryLayout();
		if (primary_layout.IsValid()) {
			primary_layout.Get()->Open_Page(InName, true, this, nullptr);
		}
	}
	return nullptr;
}

bool AMyPlayerController::Close_Popup(UMyUI_PageBase* InPage)
{
	if (auto* hud = Get_HUD()) {
		return hud->Close_Popup(InPage);
	}
	else {
		TWeakObjectPtr<UMyUI_PrimaryLayout> primary_layout = Get_PrimaryLayout();
		if (primary_layout.IsValid()) {
			primary_layout.Get()->Close_Popup(InPage);
		}
	}
	return false;
}
