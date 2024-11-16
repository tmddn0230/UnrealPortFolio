// Fill out your copyright notice in the Description page of Project Settings.

// Framework
#include "Framework/MyGameHUD.h"
#include "Framework/MyLocalPlayer.h"

AMyGameHUD::AMyGameHUD()
{
	// Basic Setting
	//if (!GIsServer) {
	//	static ConstructorHelpers::FObjectFinder<UFont>  BigFontOb(TEXT("/Game/Common/UI/Foundation/Fonts/Orbitron"));
	//	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("/Game/Common/UI/Foundation/Fonts/NotoSans"));
	//
	//	BigFont = BigFontOb.Object;
	//	NormalFont = NormalFontOb.Object;
	//}

	HUDLight = FColor(175, 202, 213, 255);
	HUDDark = FColor(110, 124, 131, 255);
}



void AMyGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (auto* player = Cast<UMyLocalPlayer>(GetNetOwningPlayer())) {
		player->CallAndRegister_OnPlayerStateSet(UMyLocalPlayer::FPlayerStateSetDelegate::FDelegate::CreateUObject(this, &AMyGameHUD::RegisterExtensionPointForPlayerState));
	}
}

void AMyGameHUD::RegisterExtensionPointForPlayerState(UMyLocalPlayer* InLocalPlayer, APlayerState* InPlayerState)
{
	// PlayerState 가 설정이 되면 HUD 를 Open
	Open_HUDWidget();
}

void AMyGameHUD::Open_HUDWidget()
{
	
}

void AMyGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}
