// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKKeybindSettingListEntry.h"
#include "RTSDKKeybindSettingsButton.h"
#include "RTSDKGameSettingKeybind.h"

void URTSDKKeybindSettingListEntry::SetSetting(UGameSetting* InSetting)
{
	Super::SetSetting(InSetting);
	URTSDKGameSettingKeybind* binddata = Cast<URTSDKGameSettingKeybind>(InSetting);
	PrimaryBindButton->SetupKeybind(binddata, true);
	SecondaryBindButton->SetupKeybind(binddata, false);
}
