// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKLobbySettingRegistry.h"

#include "Engine/LocalPlayer.h"
#include "GameSettingCollection.h"
#include "HAL/Platform.h"
#include "RTSDKGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "RTSDKLocalPlayer.h"
#include "Templates/Casts.h"
#include "RTSDKDeveloperSettings.h"
#include "RTSDKModManager.h"
#include "RTSDKLobbySubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RTSDKLobbySettingRegistry)

DEFINE_LOG_CATEGORY(LogRTSDKLobbySettingRegistry);

#define LOCTEXT_NAMESPACE "RTSDK"

URTSDKLobbySettingRegistry::URTSDKLobbySettingRegistry()
{
}

bool URTSDKLobbySettingRegistry::IsFinishedInitializing() const
{
	return Super::IsFinishedInitializing();
}

UGameSettingCollection* URTSDKLobbySettingRegistry::InitializeGameSettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	return Screen;
}

UGameSettingCollection* URTSDKLobbySettingRegistry::InitializeFactionSettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	return Screen;
}

UGameSettingCollection* URTSDKLobbySettingRegistry::InitializeMapSettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	return Screen;
}

UGameSettingCollection* URTSDKLobbySettingRegistry::InitializeMutatorSettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	return Screen;
}

void URTSDKLobbySettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	/*PlayerSettingsSubsystem = InLocalPlayer->GetSubsystem<URTSDKPlayerSettingsSubsystem>();
	InputSettings = InitializeInputSettings(InLocalPlayer);
	RegisterSetting(InputSettings);*/
}

void URTSDKLobbySettingRegistry::SaveChanges()
{
	Super::SaveChanges();
}

#undef LOCTEXT_NAMESPACE

