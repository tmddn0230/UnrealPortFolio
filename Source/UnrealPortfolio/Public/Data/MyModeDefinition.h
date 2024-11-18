// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/MyCommon.h"
#include "MyModeDefinition.generated.h"

USTRUCT()
struct FMyModeInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FName UI_Name;

	UPROPERTY(EditAnywhere)
	uint8 ShowMouseCursor : 1;

	FMyModeInfo() :UI_Name(NAME_None), ShowMouseCursor(false)
	{}
};

UCLASS()
class UNREALPORTFOLIO_API UMyModeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FMyModeInfo SingleMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ControlMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ClientMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ObserveMode;
	
	UPROPERTY(EditAnywhere)
	FMyModeInfo ServerMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo ReplayMode;

	UPROPERTY(EditAnywhere)
	FMyModeInfo DefaultMode;
public:
	FName Get_UI(EUP_PlayType InPlayType);
	const FMyModeInfo& Get_Mode(EUP_PlayType InPlayType);
	
};
