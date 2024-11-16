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
	* �̺�Ʈ ��� ���α׷��� ���� : ps ���� �� �˸��ޱ�
	* ��Ʈ��ũ�� ������ ������ ����� ���� �������� �����ϰ� �ʱ�ȭ �۵��� �� ����. �ݹ� �����ϱ�
	*/
	void RegisterExtensionPointForPlayerState(UMyLocalPlayer* InLocalPlayer, APlayerState* InPlayerState);
	void Open_HUDWidget();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	

};
