// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyUI_PageBase.h"
// UE
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Textblock.h"

#include "My_Login.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UMy_Login : public UMyUI_PageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))	UButton* Login_Btn;
	UPROPERTY(meta = (BindWidget))	UButton* Quit_Btn;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  UEditableText* UserID_var;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  UEditableText* Password_var;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Account_text;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Password_text;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Errorbox_text;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Login_text;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UTextBlock* Quit_text;

public:
	// Func
	UFUNCTION() void OnClicked_Login();
	UFUNCTION() void OnClicekd_Quit();
protected:
	virtual void NativeOnInitialized() override;
	void CheckLogin(int32 Result);
};
