// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKPlayerSettingsSubsystem.h"
#include "RTSDKGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "RTSDKDeveloperSettings.h"
#include "RTSDKGameSettingRegistry.h"
#include "Kismet/GameplayStatics.h"

void URTSDKPlayerSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ULocalPlayer* player = GetLocalPlayer<ULocalPlayer>();
	URTSDKDeveloperSettings* rtsdksettings = GetMutableDefault<URTSDKDeveloperSettings>();
	// If the save game exists, load it.
	if (UGameplayStatics::DoesSaveGameExist(RTSDK::SharedUserSettingsSaveGameSlotName, player->GetLocalPlayerIndex()))
	{
		USaveGame* Slot = UGameplayStatics::LoadGameFromSlot(RTSDK::SharedUserSettingsSaveGameSlotName, player->GetLocalPlayerIndex());
		SharedSettings = Cast<URTSDKSharedUserSettingsBase>(Slot);
	}

	if (SharedSettings == nullptr)
	{
		SharedSettings = Cast<URTSDKSharedUserSettingsBase>(UGameplayStatics::CreateSaveGameObject(rtsdksettings->SharedUserSettingsClass));
	}

	SharedSettings->Initialize(player);
	SharedSettings->ApplySettings();

	SettingsRegistry = NewObject<URTSDKGameSettingRegistry>(player, TEXT("RTSDKGameSettingRegistry"));
	SettingsRegistry->Initialize(player);
}

void URTSDKPlayerSettingsSubsystem::Deinitialize()
{
	SettingsRegistry = nullptr;
}

URTSDKGameSettingRegistry* URTSDKPlayerSettingsSubsystem::GetSettingsRegistry()
{
	return SettingsRegistry;
}

UGameUserSettings* URTSDKPlayerSettingsSubsystem::GetLocalSettings() const
{
	return GetMutableDefault<UGameUserSettings>();
}

URTSDKSharedUserSettingsBase* URTSDKPlayerSettingsSubsystem::GetSharedSettings() const
{
	return SharedSettings;
}