// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameSettingKeybind.h"
#include "RTSDKGameUserSettings.h"

#define LOCTEXT_NAMESPACE "RTSDK"

void URTSDKGameSettingKeybind::StoreInitial()
{
	InitialBinds = Binds;
}

void URTSDKGameSettingKeybind::ResetToDefault()
{
	Binds = DefaultBinds;
}

void URTSDKGameSettingKeybind::RestoreToInitial()
{
	Binds = InitialBinds;
}

void URTSDKGameSettingKeybind::OnInitialized()
{
}

void URTSDKGameSettingKeybind::OnApply()
{

}

void URTSDKGameSettingKeybind::InitializeDefaults(FName inContextName, FName inBindName, const FRTSDKSavedBindPair& inDefaultBinds, URTSDKKeyBindMetadata* inMetadata)
{
	DefaultBinds = inDefaultBinds;
	KeybindMetadata = inMetadata;
	ContextName = inContextName;
	BindName = inBindName;
}

FText URTSDKGameSettingKeybind::GetBindDisplayName() const
{
	return Binds.DisplayName;
}

FText URTSDKGameSettingKeybind::GetBindTooltip() const
{
	return Binds.TooltipText;
}

FText URTSDKGameSettingKeybind::GetBoundKeysDisplayText(bool bPrimaryBind) const
{
	TArray<FText> displaynames;
	if (bPrimaryBind)
	{
		if (Binds.Primary.BoundKeys.Num() > 0)
		{
			for (int32 i = 0; i < Binds.Primary.BoundKeys.Num(); i++)
			{
				displaynames.Add(Binds.Primary.BoundKeys[i].GetDisplayName());
			}
			return FText::Join(FText::FromString(TEXT(", ")), displaynames);
		}
		return LOCTEXT("InputSettingsNoBind", "Not Bound");
	}
	else
	{
		if (Binds.Secondary.BoundKeys.Num() > 0)
		{
			for (int32 i = 0; i < Binds.Secondary.BoundKeys.Num(); i++)
			{
				displaynames.Add(Binds.Secondary.BoundKeys[i].GetDisplayName());
			}
			return FText::Join(FText::FromString(TEXT(", ")), displaynames);
		}
		return LOCTEXT("InputSettingsNoBind", "Not Bound");
	}
}

TArray<FKey> URTSDKGameSettingKeybind::GetBoundKeys(bool bPrimaryBind) const
{
	return bPrimaryBind ? Binds.Primary.BoundKeys : Binds.Secondary.BoundKeys;
}

void URTSDKGameSettingKeybind::OnInputProfileChanged()
{
	FRTSDKInputSettingsProfile activeprofile = SharedSettings->GetActiveInputProfile();
	FRTSDKSavedBindContext activecontext = activeprofile.GetContextByName(ContextName);
	FRTSDKSavedBindPair activebind = activecontext.GetBindsByName(BindName);
	if (activebind.BindName == BindName)
	{
		Binds = activebind;
	}
	else
	{
		Binds = DefaultBinds;
	}
	//todo: apply the setting by getting the local player's enhance input instance and getting the actions
	//from the actions, you can get the modifiers for the responsible bind
	//then you can set the bound primary key and put additional keys in the additional keys modifier
}

void URTSDKGameSettingKeybind::SetBoundKeys(const TArray<FKey>& inKeys, bool bPrimaryBind)
{
	bPrimaryBind ? Binds.Primary.BoundKeys = inKeys : Binds.Secondary.BoundKeys = inKeys;
}

void URTSDKGameSettingKeybind::ClearBoundKeys(bool bPrimaryBind)
{
	bPrimaryBind ? Binds.Primary.BoundKeys.Empty() : Binds.Secondary.BoundKeys.Empty();
}

void URTSDKGameSettingKeybind::InitializeBinds(const FRTSDKSavedBindPair& inBinds, URTSDKSharedUserSettingsBase* inSharedSettings)
{
	SharedSettings = inSharedSettings;
	SharedSettings->OnInputProfileChanged.AddDynamic(this, &URTSDKGameSettingKeybind::OnInputProfileChanged);
	if (inBinds.BindName == BindName)
	{
		Binds = inBinds;
	}
	else
	{
		Binds = DefaultBinds;
	}
}

#undef LOCTEXT_NAMESPACE
