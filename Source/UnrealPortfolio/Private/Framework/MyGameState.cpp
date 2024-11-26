// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyGameState.h"
//Unreal Basic Framework
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
// Framework
#include "Framework/MyPlayerState.h"

AMyGameState::FAdd_PlayerState          AMyGameState::Handle_AddPlayerState;
AMyGameState::FRemove_PlayerState       AMyGameState::Handle_RemovePlayerState;
AMyGameState::FChanged_GameScenarioInfo AMyGameState::Handle_ChangedScenarioInfo;
AMyGameState::FChanged_Devicenfo        AMyGameState::Handle_ChangedDeviceInfo;
AMyGameState::FChanged_TraineeInfo      AMyGameState::Handle_ChangedTraineeInfo;

AMyGameState::AMyGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
}


AMyPlayerState* AMyGameState::Find_AssignedDevice(const FString& InTrainee)
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<AMyPlayerState>(it.Get())) {
			if (ps->Login_PlayerName.Equals(InTrainee)) {
				return ps;
			}
		}
	}
	return nullptr;
}

void AMyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	Handle_AddPlayerState.Broadcast(PlayerState);
}

void AMyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	Handle_RemovePlayerState.Broadcast(PlayerState);
}

void AMyGameState::OnRep_Scenario_Info()
{
	Handle_ChangedScenarioInfo.Broadcast(this);
}

void AMyGameState::OnRep_Device_Info()
{
	Handle_ChangedDeviceInfo.Broadcast(this);
}

void AMyGameState::OnRep_Trainee_Info()
{
	Handle_ChangedTraineeInfo.Broadcast(this);
}

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, Scenario_Info);
	DOREPLIFETIME(AMyGameState, Device_Info);
	DOREPLIFETIME(AMyGameState, Trainee_Info);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
}
