// Fill out your copyright notice in the Description page of Project Settings.

// Framework
#include "Framework/MyGameHUD.h"
#include "Framework/MyLocalPlayer.h"
#include "Framework/MyPlayerController.h"
// Data
#include "Data/MyNames.h"
#include "Data/MyTableManager.h"
#include "Data/MyModeDefinition.h"
// Manager
#include "Manager/MyGameManager.h"
// UI
#include "UI/MyUI_PrimaryLayout.h"

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
	Init_PrimaryLayer();

	UMyUI_PageBase* res = nullptr;
	auto* pc = GetOwningPlayerController();
	if (auto* mgr = Get_GameManager()) {

		if (auto ModeDefinition = mgr->ModeDefinition.Get()) {
			auto play_type = mgr->Get_PlayType();
		
			auto mode = ModeDefinition->Get_Mode(play_type);
		
			if (mode.UI_Name != NAME_None) {
				res = Open_Page(mode.UI_Name, false);
				if (res) {
					res->Post_OpenPage();
				}
			}
			GetOwningPlayerController()->SetShowMouseCursor(mode.ShowMouseCursor);
		}
	}
}

bool AMyGameHUD::Init_PrimaryLayer()
{
	if (Primary_Layout.Get())
		return true;
	if (auto* mgr = Get_GameManager()) {
		if (auto* table = Get_TableManager()) {
			if (auto widget_class = table->Get_UI_Widget(MyName_UI::PrimaryLayout)) {
				if (UMyUI_PrimaryLayout* widget = CreateWidget<UMyUI_PrimaryLayout>(GetOwningPlayerController(), widget_class)) {
					widget->AddToViewport();
					Primary_Layout = widget;
					return true;
				}
			}
		}
	}
	return false;
}

UMyUI_PageBase* AMyGameHUD::Open_Page(const FName& InName, bool InPopup)
{
	if (Primary_Layout.Get()) {
		return Primary_Layout.Get()->Open_Page(InName, InPopup, Get_PlayerController(), nullptr);
	}
	return nullptr;
}

bool AMyGameHUD::Close_Popup(UMyUI_PageBase* InPage)
{
	if (Primary_Layout.Get()) {
		return Primary_Layout.Get()->Close_Popup(InPage);
	}
	return false;
}

void AMyGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UMyGameManager* AMyGameHUD::Get_GameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<UMyGameManager>();

	return nullptr;
}

UMyTableManager* AMyGameHUD::Get_TableManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<UMyTableManager>();

	return nullptr;
}

AMyPlayerController* AMyGameHUD::Get_PlayerController()
{
	return Cast<AMyPlayerController>(GetOwningPlayerController());
}
