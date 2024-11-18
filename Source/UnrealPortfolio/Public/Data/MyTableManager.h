// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "Data/MyTableRows.h"
#include "MyTableManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMyTableManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMyTableManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
};
