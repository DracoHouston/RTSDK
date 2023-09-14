// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSDKKeybindSettingsButton.h"
#include "RTSDKButton.h"
#include "RTSDKGameSettingKeybind.h"
#include "CommonRichTextBlock.h"

void URTSDKKeybindSettingsButton::SetupKeybind(URTSDKGameSettingKeybind* inBindData, bool inIsPrimaryBind)
{
	BindData = inBindData;
	bPrimaryBind = inIsPrimaryBind;
	BoundKeysRichText->SetText(BindData->GetBoundKeysDisplayText(bPrimaryBind));
	BindButton->OnLeftMouseClicked.AddDynamic(this, &ThisClass::OnLeftClicked);
	BindButton->OnRightMouseClicked.AddDynamic(this, &ThisClass::OnRightClicked);
	BindButton->OnMiddleMouseClicked.AddDynamic(this, &ThisClass::OnMiddleClicked);
	BindData->OnSettingChangedEvent.AddUObject(this, &ThisClass::OnSettingChanged);
}

void URTSDKKeybindSettingsButton::OnLeftClicked()
{
	//set bind screen
}

void URTSDKKeybindSettingsButton::OnRightClicked()
{
	//shell menu thingy
}

void URTSDKKeybindSettingsButton::OnMiddleClicked()
{
	BindData->ClearBoundKeys(bPrimaryBind);
}

void URTSDKKeybindSettingsButton::OnSettingChanged(UGameSetting* InSetting, EGameSettingChangeReason InChangeReason)
{
	BoundKeysRichText->SetText(BindData->GetBoundKeysDisplayText(bPrimaryBind));
}
