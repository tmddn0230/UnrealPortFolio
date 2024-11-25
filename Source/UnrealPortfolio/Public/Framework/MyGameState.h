// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
// Sphinx
#include "SpeechRecognitionGameStateBase.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AMyGameState : public ASpeechRecognitionGameStateBase
{
	GENERATED_BODY()

protected:

	//UPROPERTY(ReplicatedUsing = OnRep_Scenario_Info)	FMyScenarioInfo Scenario_Info;
	//UPROPERTY(ReplicatedUsing = OnRep_Device_Info)  TArray<FRtDeviceInfo> Device_Info;
	//UPROPERTY(ReplicatedUsing = OnRep_Player_Info) TArray<FRtTraineeInfo> Trainee_Info;
	
public:
	AMyGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
