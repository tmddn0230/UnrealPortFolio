// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyLocalPlayer.h"

UMyLocalPlayer::UMyLocalPlayer()
	: Super(FObjectInitializer::Get())
{
}

FDelegateHandle UMyLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;

	if (PlayerState)
	{
		Delegate.Execute(this, PlayerState);
	}

	return OnPlayerStateSet.Add(Delegate);
}

FDelegateHandle UMyLocalPlayer::CallAndRegister_OnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());

	if (PC)
	{
		Delegate.Execute(this, PC);
	}

	return OnPlayerControllerSet.Add(Delegate);
}
