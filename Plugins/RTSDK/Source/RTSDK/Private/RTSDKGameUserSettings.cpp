// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameUserSettings.h"
#include "RTSDKLocalPlayer.h"
#include "RTSDKDeveloperSettings.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/Gameplaystatics.h"


//
//URTSDKDeveloperSettings* rtsdksettings = GetMutableDefault<URTSDKDeveloperSettings>();
//InputProfiles.Insert(rtsdksettings->DefaultInputProfile, 0);

URTSDKSharedUserSettingsBase::URTSDKSharedUserSettingsBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	URTSDKDeveloperSettings* rtsdksettings = GetMutableDefault<URTSDKDeveloperSettings>();
	InputProfiles.Insert(rtsdksettings->DefaultInputProfile, 0);
	ActiveProfile = 0;
}

void URTSDKSharedUserSettingsBase::Initialize(ULocalPlayer* LocalPlayer)
{
	check(LocalPlayer);

	OwningPlayer = LocalPlayer;
}

FRTSDKInputSettingsProfile URTSDKSharedUserSettingsBase::GetActiveInputProfile()
{
	if (InputProfiles.Num() == 0)
	{
		return FRTSDKInputSettingsProfile();
	}
	if ((ActiveProfile >= 0) && (ActiveProfile < InputProfiles.Num()))
	{
		return InputProfiles[ActiveProfile];
	}
	return InputProfiles[0];
}

FRTSDKInputSettingsProfile& URTSDKSharedUserSettingsBase::GetMutableActiveInputProfile()
{
	if ((ActiveProfile >= 0) && (ActiveProfile < InputProfiles.Num()))
	{
		return InputProfiles[ActiveProfile];
	}
	return InputProfiles[0];
}

void URTSDKSharedUserSettingsBase::SetBindForActiveProfile(FName inBindName, FName inContextName, const TArray<FKey>& inKeys, bool inbIsPrimary)
{
	GetMutableActiveInputProfile().GetMutableContextByName(inContextName).GetMutableBindsByName(inBindName).GetMutableBind(inbIsPrimary).BoundKeys = inKeys;
	
}

void URTSDKSharedUserSettingsBase::SetActiveInputProfileByIndex(int32 ProfileIndex)
{
	if ((ProfileIndex >= 0) && (ProfileIndex < InputProfiles.Num()))
	{
		ActiveProfile = ProfileIndex;
		OnInputProfileChanged.Broadcast();
	}
}

void URTSDKSharedUserSettings::SaveSettings()
{
	check(OwningPlayer);
	UGameplayStatics::SaveGameToSlot(this, RTSDK::SharedUserSettingsSaveGameSlotName, OwningPlayer->GetLocalPlayerIndex());
}

void URTSDKSharedUserSettings::ApplySettings()
{

}
