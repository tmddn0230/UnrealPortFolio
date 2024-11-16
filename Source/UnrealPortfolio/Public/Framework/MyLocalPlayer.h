// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "MyLocalPlayer.generated.h"

/**
 * Network ���ӿ��� Player Controller ��ŭ �߿��� Ŭ����
 * �������� ���� Ŭ���̾�Ʈ���� �����ϸ� Ŭ���̾�Ʈ�� ��ü�� ��Ÿ�� 
 * ���ӷ������� Ŭ���̾�Ʈ �Է�ó���� ���⼭ �ϴ� ��
 * 
 * ���⼭ PlayerState�� PlayerController �� ������ ȣ���� ���
 * UGameInstance�� ULocalPlayer�� �迭�� �����ϸ�, �̸� ���� ���� ��ü���� �÷��̾� ������ ���� ����
 * ������ ���� �����̳� ���� ���� �÷��̾ ���¸� ULocalPlayer�� �����Ͽ� ������ �� ����
 */
UCLASS()
class UNREALPORTFOLIO_API UMyLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	
public:
	UMyLocalPlayer();


	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, UMyLocalPlayer* LocalPlayer, APlayerState* PlayerState);
	FPlayerStateSetDelegate OnPlayerStateSet;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UMyLocalPlayer* LocalPlayer, APlayerController* PlayerController);
	FPlayerControllerSetDelegate OnPlayerControllerSet;


	FDelegateHandle CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate);
	FDelegateHandle CallAndRegister_OnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate);
};
