// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
// Common
#include "Common/MyCommon.h"
#include "MyPlayerController.generated.h"


class AMyGameMode;
class AMyGameHUD;
class AMyPlayerState;

UCLASS()
class UNREALPORTFOLIO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	// Replicate
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Get
	AMyGameMode*    Get_GameMode();         // for Only Server
	AMyGameHUD*     Get_HUD();
	AMyPlayerState* Get_PlayerState();
	EUP_PlayType    Get_PlayType();

};
