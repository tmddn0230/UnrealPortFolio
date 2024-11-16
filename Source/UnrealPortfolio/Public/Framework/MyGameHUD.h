// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyGameHUD.generated.h"

class UMyLocalPlayer;

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
	
public:
	AMyGameHUD();
	// Open
	/*
	* 이벤트 기반 프로그래밍 패턴 : ps 설정 시 알림받기
	* 네트워크의 문제로 지연이 길어질 때도 문제없이 안전하게 초기화 작동할 수 있음. 콜백 구조니까
	*/
	void RegisterExtensionPointForPlayerState(UMyLocalPlayer* InLocalPlayer, APlayerState* InPlayerState);
	void Open_HUDWidget();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	

};
