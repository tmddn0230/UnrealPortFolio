// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
// UE
#include "Kismet/Gameplaystatics.h"
#include "AbilitySystemInterface.h"
// Common
#include "Common/MyCommon.h"
#include "MyPlayerState.generated.h"

class AMyPlayerController;

UCLASS()
class UNREALPORTFOLIO_API AMyPlayerState : public APlayerState// , public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(Replicated)
	EUP_PlayType PlayType;
	
//public:
//	AMyPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
//
//	// Replicate
//	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//
//	/** Called by Controller when its PlayerState is initially replicated. */
//	virtual void ClientInitialize(class AController* C) override;
//public:
//	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
//protected:
//	virtual void CopyProperties(APlayerState* PlayerState) override;

};
