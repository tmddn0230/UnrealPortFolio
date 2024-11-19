// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/My_Login.h"
// UE
#include "Kismet/GameplayStatics.h"
// Network
#include "Network/MyHttpManager.h"

void UMy_Login::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Login_Btn) {
		Login_Btn->OnClicked.AddDynamic(this, &UMy_Login::OnClicked_Login);
	}
}


void UMy_Login::OnClicked_Login()
{
	// Connect Web
	if (auto* HttpSubsys = GetHttpManager())
	{
		FString InId;
		FText UserID = UserID_var->GetText();
		InId = UserID.ToString();

		FString InPass;
		FText Password = Password_var->GetText();
		InPass = Password.ToString();
		HttpSubsys->Login(InId, InPass);
		HttpSubsys->Handle_LoginCheck.AddUFunction(this, FName("CheckLogin"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Invalid Web Subsystem !! "));
		return;
	}
}


void UMy_Login::CheckLogin(int32 Result)
{
	switch (Result)
	{
	case (int32)EWebProtocolType::webErrorServerCheck:

		break;
	case (int32)EWebProtocolType::webErrorBlock:

		break;
	case (int32)EWebProtocolType::WebError:

		break;
	case (int32)EWebProtocolType::webErrorDup:

		break;
	case (int32)EWebProtocolType::webErrorParse:

		break;
	case 1 :                                                //Login Success!! 
		BPCall_Open_Page("ScenarioSelect");
		break;
	default:
		break;
	}
}



