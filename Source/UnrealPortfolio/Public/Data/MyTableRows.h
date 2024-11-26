// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
// Common
#include "Common/MyCommon.h"
#include "MyTableRows.generated.h"


USTRUCT(BlueprintInternalUseOnly)
struct FMyTableRow_UI : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere)		TSubclassOf<class UUserWidget> WidgetClass;
	UPROPERTY(EditAnywhere)		FString Desc;
};

USTRUCT(BlueprintInternalUseOnly)
struct FMessageBoxButtonAction
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")	EUP_MessageResult Result = EUP_MessageResult::Unknown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")	FText Text;
};

USTRUCT(BlueprintInternalUseOnly)
struct FTableRow_MessageBox : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")	FText Header;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")	FText Body;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")	FString DebugText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")	TArray<FMessageBoxButtonAction> ButtonArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")   UTexture2D* Texture_Normal;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")   UTexture2D* Texture_Hover;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnrealPortfolio")   UTexture2D* Texture_Cancel;

};

class UNREALPORTFOLIO_API MyTableRows
{
public:
	MyTableRows();
	~MyTableRows();
};
