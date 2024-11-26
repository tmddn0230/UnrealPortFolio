// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
// Common : RPC
#include "Common/MyRpcCommon.h"
// Sphinx
#include "SpeechRecognitionGameStateBase.h"
#include "MyGameState.generated.h"

class AMyPlayerState;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AMyGameState : public ASpeechRecognitionGameStateBase
{
	GENERATED_BODY()

protected:

	// 프로젝트 내에서 관리해야하는 중요 데이터 구조
	UPROPERTY(ReplicatedUsing = OnRep_Scenario_Info) FScenarioInfo        Scenario_Info;
	UPROPERTY(ReplicatedUsing = OnRep_Device_Info)   TArray<FDeviceInfo>  Device_Info;
	UPROPERTY(ReplicatedUsing = OnRep_Trainee_Info)  TArray<FTraineeInfo> Trainee_Info;

public:
	// 데이터 변동시 호출할 델리게이트 핸들러
	DECLARE_MULTICAST_DELEGATE_OneParam(FAdd_PlayerState, APlayerState*);
	static	FAdd_PlayerState         Handle_AddPlayerState;

	DECLARE_MULTICAST_DELEGATE_OneParam(FRemove_PlayerState, APlayerState*);
	static  FRemove_PlayerState      Handle_RemovePlayerState;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_GameScenarioInfo, AMyGameState*);
	static FChanged_GameScenarioInfo Handle_ChangedScenarioInfo;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_Devicenfo, AMyGameState*);
	static FChanged_Devicenfo        Handle_ChangedDeviceInfo;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_TraineeInfo, AMyGameState*);
	static FChanged_TraineeInfo      Handle_ChangedTraineeInfo;

	
public:
	AMyGameState(const FObjectInitializer& ObjectInitializer);
	// Assign
	UFUNCTION(BlueprintCallable)
	AMyPlayerState* Find_AssignedDevice(const FString& InTrainee);
	/** Add PlayerState to the PlayerArray */
	virtual void    AddPlayerState(APlayerState* PlayerState) override;
	/** Remove PlayerState from the PlayerArray. */
	virtual void    RemovePlayerState(APlayerState* PlayerState) override;

	// OnRep
	UFUNCTION()	void OnRep_Scenario_Info();
	UFUNCTION() void OnRep_Device_Info();
	UFUNCTION() void OnRep_Trainee_Info();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



};
