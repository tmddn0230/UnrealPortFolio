// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUI_UserWidget.generated.h"

class UMyGameManager;
class AMyPlayerController;
class AMyGameState;
class AMyGameHUD;
class AMyGameMode;

UCLASS()
class UNREALPORTFOLIO_API UMyUI_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// Function
public:
	// Get
	UMyGameManager*      GetGameManager();
	AMyPlayerController* Get_PlayerController();
	AMyGameState*        Get_GameState();
	AMyGameMode*         Get_GameMode();
	AMyGameHUD*          Get_HUD();

	// bool Check : Player
	bool IsControl_PlayType();
	bool IsLocalPlayer();
	bool IsLocalConllerPlayType();


};
