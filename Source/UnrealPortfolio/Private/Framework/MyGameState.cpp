// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyGameState.h"
//Unreal Basic Framework
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AMyGameState::AMyGameState()
{
	
}

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	//DOREPLIFETIME(AMyGameState, Training_Index);
	//DOREPLIFETIME(AMyGameState, RemainTime);
	//DOREPLIFETIME(AMyGameState, Scenario_Info);
	//DOREPLIFETIME(AMyGameState, Device_Info);
	//DOREPLIFETIME(AMyGameState, Trainee_Info);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
}
