// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUI_PageBase.h"
// Framework
#include "Framework/MyPlayerController.h"

// Log
DEFINE_LOG_CATEGORY(LogPage_Base);

UMyUI_PageBase* UMyUI_PageBase::BPCall_Open_Popup(const FName& InName)
{
    return Open_Popup(InName);
}

bool UMyUI_PageBase::BPCall_Close_Popup(UMyUI_PageBase* InPage)
{
    return Close_Popup(InPage);
}

UMyUI_PageBase* UMyUI_PageBase::BPCall_Open_Page(const FName& InName)
{
    return Open_Page(InName);
}

UMyUI_PageBase* UMyUI_PageBase::Open_Popup(const FName& InName)
{
    return nullptr;
}

bool UMyUI_PageBase::Close_Popup(UMyUI_PageBase* InPage)
{
    if (auto* pc = Get_PlayerController()) {
        //return pc->Close_Popup(InPage);
    }
    return false;
}

UMyUI_PageBase* UMyUI_PageBase::Open_Page(const FName& InName)
{
    if (auto* pc = Get_PlayerController()) {
        //return pc->Open_Page(InName);
    }
    return nullptr;
}

void UMyUI_PageBase::Close_Page()
{
}
