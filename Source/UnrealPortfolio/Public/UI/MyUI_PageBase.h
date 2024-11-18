// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUI_UserWidget.h"
#include "MyUI_PageBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMyUI_PageBase : public UMyUI_UserWidget
{
	GENERATED_BODY()
	
public:
	// BPCall
	UFUNCTION(BlueprintCallable)	UMyUI_PageBase* BPCall_Open_Popup(const FName& InName);
	UFUNCTION(BlueprintCallable)	bool            BPCall_Close_Popup(UMyUI_PageBase* InPage);
	UFUNCTION(BlueprintCallable)	UMyUI_PageBase* BPCall_Open_Page(const FName& InName);
	
	// virtual redefine
	virtual UMyUI_PageBase* Open_Popup(const FName& InName);
	virtual bool            Close_Popup(UMyUI_PageBase* InPage);
	virtual UMyUI_PageBase* Open_Page(const FName& InName);
	virtual void            Close_Page();
	
};
