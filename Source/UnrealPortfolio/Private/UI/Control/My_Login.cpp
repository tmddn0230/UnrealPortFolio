// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/My_Login.h"
// UE
#include "Kismet/GameplayStatics.h"
// Network
#include "Network/MyHttpManager.h"
// Common
#include "Common/MyConfig.h"
// Framework
#include "Framework/MyGameHUD.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
//#include "Internationalization/StringTableRegistry.h"


void UMy_Login::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Login_Btn) {
		Login_Btn->OnClicked.AddDynamic(this, &UMy_Login::OnClicked_Login);
	}

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *UMyConfig::ControlMainPath);
	UStringTable* ErrorMsgBoxTable =   LoadObject<UStringTable>(nullptr, *UMyConfig::ErrorMsgPath);
	if (ControlStringTable == nullptr) return;
	if (ErrorMsgBoxTable == nullptr) return;
	if (Account_text && Password_text && Errorbox_text && Login_text && Quit_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Account"), OUTPUT);
		Account_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Password"), OUTPUT);
		Password_text->SetText(FText::FromString(OUTPUT));

		ErrorMsgBoxTable->GetMutableStringTable().Get().GetSourceString(FTextKey("LogError"), OUTPUT);
		Errorbox_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Login"), OUTPUT);
		Login_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Quit"), OUTPUT);
		Quit_text->SetText(FText::FromString(OUTPUT));
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

void UMy_Login::OnClicekd_Quit()
{
	// 정말 종료하시겠습니까? 팝업 창 
	if (auto* hud = Get_HUD()) {
		FMyDele_MessageResult ResultCallback = FMyDele_MessageResult::CreateLambda([this](EUP_MessageResult ConfirmationResult) {
			if (ConfirmationResult == EUP_MessageResult::Confirmed) {
				// TODO :
				// 확인 버튼을 눌렀을 때 처리할 코드
			}
			else if (ConfirmationResult == EUP_MessageResult::Cancelled) {
				// TODO :
				// 취소 버튼을 눌렀을 때 처리할 코드
			}
			else {
				// TODO : 
				// 기타
			}
			});
		hud->OpenMsgBox_Popup("AssignedTrainee", ResultCallback);
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



