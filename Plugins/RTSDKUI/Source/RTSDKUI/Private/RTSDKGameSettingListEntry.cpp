// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameSettingListEntry.h"
#include "CommonRichTextBlock.h"
#include "GameSetting.h"

void URTSDKGameSettingListEntry::SetSetting(UGameSetting* InSetting)
{
	Super::SetSetting(InSetting);
	SettingsDisplayNameText->SetText(InSetting->GetDisplayName());
}

void URTSDKGameSettingListEntry::SetDisplayNameOverride(const FText& OverrideName)
{
	Super::SetDisplayNameOverride(OverrideName);
	SettingsDisplayNameText->SetText(OverrideName);
}
