// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUI_PageBase.h"

UMyUI_PageBase* UMyUI_PageBase::BPCall_Open_Popup(const FName& InName)
{
    return nullptr;
}

bool UMyUI_PageBase::BPCall_Close_Popup(UMyUI_PageBase* InPage)
{
    return false;
}

UMyUI_PageBase* UMyUI_PageBase::BPCall_Open_Page(const FName& InName)
{
    return nullptr;
}

UMyUI_PageBase* UMyUI_PageBase::Open_Popup(const FName& InName)
{
    return nullptr;
}

bool UMyUI_PageBase::Close_Popup(UMyUI_PageBase* InPage)
{
    return false;
}

UMyUI_PageBase* UMyUI_PageBase::Open_Page(const FName& InName)
{
    return nullptr;
}

void UMyUI_PageBase::Close_Page()
{
}
