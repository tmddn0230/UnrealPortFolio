// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
// Common
#include "Common/MyCommon.h" // for Delegate
#include "MyGameHUD.generated.h"

class UMyGameManager;
class UMyTableManager;

class UMyLocalPlayer;
class AMyPlayerController;
class UMyUI_PageBase;
class UMyUI_PrimaryLayout;

UCLASS()
class UNREALPORTFOLIO_API AMyGameHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	// Default Settings
	UPROPERTY()	UFont* BigFont;
	UPROPERTY()	UFont* NormalFont;
	FColor HUDDark;
	FColor HUDLight;
	float ScaleUI;
	
	TWeakObjectPtr<UMyUI_PrimaryLayout> Primary_Layout;

public:
	AMyGameHUD();
	// Open
	/*
	* 이벤트 기반 프로그래밍 패턴 : ps 설정 시 알림받기
	* 네트워크의 문제로 지연이 길어질 때도 콜백 구조로 안전하게 초기화 작동할 수 있음. 
	*/
	void RegisterExtensionPointForPlayerState(UMyLocalPlayer* InLocalPlayer, APlayerState* InPlayerState);
	void Open_HUDWidget();

	// UI Widget 
	bool             Init_PrimaryLayer();
	UMyUI_PageBase*  Open_Page(const FName& InName, bool InPopup);
	bool             Close_Popup(UMyUI_PageBase* InPage);
	void             OpenMsgBox_Popup(const FName& InNmae, FMyDele_MessageResult ResultCallback);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
public:
	// Get
	UMyGameManager*      Get_GameManager();
	UMyTableManager*     Get_TableManager();
	AMyPlayerController* Get_PlayerController();

};
