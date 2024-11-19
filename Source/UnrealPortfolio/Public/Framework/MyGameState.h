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

public:
	AMyGameState();
};
