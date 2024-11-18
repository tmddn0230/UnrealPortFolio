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
	UPROPERTY()	UDataTable* Widget_Table;
	UPROPERTY()	UDataTable* MessageBox_Table;
	
public:
	UMyTableManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	// Get
	FMyTableRow_UI*          Get_UI(const FName& row_name);
	FTableRow_MessageBox*    Get_MessageBox(const FName& row_name);
	TSubclassOf<UUserWidget> Get_UI_Widget(const FName& row_name);

};
