// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "MyLocalPlayer.generated.h"

/**
 * Network 게임에서 Player Controller 만큼 중요한 클래스
 * 서버에는 없고 클라이언트에만 존재하며 클라이언트의 객체를 나타냄 
 * 게임로직외의 클라이언트 입력처리를 여기서 하는 것
 * 
 * 여기서 PlayerState와 PlayerController 의 설정시 호출을 담당
 * UGameInstance는 ULocalPlayer의 배열을 관리하며, 이를 통해 게임 전체에서 플레이어 정보에 접근 가능
 * 게임의 현재 레벨이나 모드와 같은 플레이어별 상태를 ULocalPlayer에 저장하여 관리할 수 있음
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
