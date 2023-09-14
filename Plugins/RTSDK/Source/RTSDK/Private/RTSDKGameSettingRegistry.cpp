// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKGameSettingRegistry.h"

#include "Engine/LocalPlayer.h"
#include "GameSettingCollection.h"
#include "HAL/Platform.h"
#include "RTSDKGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "RTSDKLocalPlayer.h"
#include "Templates/Casts.h"
#include "RTSDKPlayerSettingsSubsystem.h"
#include "RTSDKDeveloperSettings.h"
#include "RTSDKGameSettingKeybind.h"
#include "RTSDKKeyBindMetadata.h"
#include "RTSDKModManager.h"
#include "InputMappingContext.h"
#include "RTSDKConfigurableInputMappingDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RTSDKGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogRTSDKGameSettingRegistry);

#define LOCTEXT_NAMESPACE "RTSDK"

URTSDKGameSettingRegistry::URTSDKGameSettingRegistry()
{
}

bool URTSDKGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (PlayerSettingsSubsystem != nullptr)
		{
			if (PlayerSettingsSubsystem->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

UGameSettingCollection* URTSDKGameSettingRegistry::InitializeInputSettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("InputSettingsCollection"));
	Screen->SetDisplayName(LOCTEXT("InputSettingsCollection_Name", "Input Settings"));
	Screen->Initialize(InLocalPlayer);
	URTSDKSharedUserSettingsBase* sharedsettings = PlayerSettingsSubsystem->GetSharedSettings();
	FRTSDKInputSettingsProfile activeprofile = sharedsettings->GetActiveInputProfile();
	URTSDKDeveloperSettings* devsettings = GetMutableDefault<URTSDKDeveloperSettings>();
	if (!devsettings->GlobalMappingContext.IsNull())
	{
		UGameSettingCollection* GlobalBindContext = NewObject<UGameSettingCollection>();
		UInputMappingContext* globalbinds = devsettings->GlobalMappingContext.LoadSynchronous();
		FRTSDKSavedBindContext globalcontextdefault = FRTSDKSavedBindContext(globalbinds);
		FRTSDKSavedBindContext globalcontextlive = activeprofile.GetContextByName(globalcontextdefault.ContextName);
		GlobalBindContext->SetDevName(globalcontextdefault.ContextName);
		GlobalBindContext->SetDisplayName(globalcontextdefault.DisplayName);
		GlobalBindContext->Initialize(InLocalPlayer);
		for (int32 b = 0; b < globalcontextdefault.PlayerBinds.Num(); b++)
		{
			URTSDKGameSettingKeybind* bind = NewObject<URTSDKGameSettingKeybind>();
			bind->InitializeDefaults(
				globalcontextdefault.ContextName,
				globalcontextdefault.PlayerBinds[b].BindName,
				globalcontextdefault.PlayerBinds[b],
				globalcontextdefault.PlayerBinds[b].BindMetadata.Get());
			FRTSDKSavedBindPair activebind = globalcontextlive.GetBindsByName(globalcontextdefault.PlayerBinds[b].BindName);
			bind->InitializeBinds(activebind, sharedsettings);
			bind->SetDevName(globalcontextdefault.PlayerBinds[b].BindName);
			bind->SetDisplayName(globalcontextdefault.PlayerBinds[b].DisplayName);
			bind->Initialize(InLocalPlayer);
			GlobalBindContext->AddSetting(bind);
		}
		Screen->AddSetting(GlobalBindContext);
	}
	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();

	TArray<URTSDKConfigurableInputMappingDefinition*> modmappings = modmanager->GetAllValidInputMappings();

	for (int32 i = 0; i < modmappings.Num(); i++)
	{
		FRTSDKSavedBindContext contextlive = activeprofile.GetContextByName(modmappings[i]->MappingContextData.ContextName);
		UGameSettingCollection* BindContext = NewObject<UGameSettingCollection>();
		BindContext->SetDevName(modmappings[i]->MappingContextData.ContextName);
		BindContext->SetDisplayName(modmappings[i]->MappingContextData.DisplayName);
		BindContext->Initialize(InLocalPlayer);
		for (int32 b = 0; b < modmappings[i]->MappingContextData.PlayerBinds.Num(); b++)
		{
			modmappings[i]->MappingContextData.PlayerBinds;
			URTSDKGameSettingKeybind* bind = NewObject<URTSDKGameSettingKeybind>();
			bind->InitializeDefaults(
				modmappings[i]->MappingContextData.ContextName,
				modmappings[i]->MappingContextData.PlayerBinds[b].BindName,
				modmappings[i]->MappingContextData.PlayerBinds[b],
				modmappings[i]->MappingContextData.PlayerBinds[b].BindMetadata.Get());
			FRTSDKSavedBindPair activebind = contextlive.GetBindsByName(modmappings[i]->MappingContextData.PlayerBinds[b].BindName);
			bind->InitializeBinds(activebind, sharedsettings);
			bind->SetDevName(modmappings[i]->MappingContextData.PlayerBinds[b].BindName);
			bind->SetDisplayName(modmappings[i]->MappingContextData.PlayerBinds[b].DisplayName);
			bind->Initialize(InLocalPlayer);
			BindContext->AddSetting(bind);
		}
	}

	return Screen;
}

void URTSDKGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	PlayerSettingsSubsystem = InLocalPlayer->GetSubsystem<URTSDKPlayerSettingsSubsystem>();
	InputSettings = InitializeInputSettings(InLocalPlayer);
	RegisterSetting(InputSettings);
}

void URTSDKGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (PlayerSettingsSubsystem != nullptr)
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		PlayerSettingsSubsystem->GetLocalSettings()->ApplySettings(false);
		
		PlayerSettingsSubsystem->GetSharedSettings()->ApplySettings();
		PlayerSettingsSubsystem->GetSharedSettings()->SaveSettings();
	}
}

#undef LOCTEXT_NAMESPACE

