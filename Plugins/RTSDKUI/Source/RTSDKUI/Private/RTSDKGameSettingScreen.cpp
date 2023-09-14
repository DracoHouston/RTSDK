// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameSettingScreen.h"
#include "RTSDKPlayerSettingsSubsystem.h"
#include "RTSDKGameSettingRegistry.h"
#include "Input/CommonUIInputTypes.h"

void URTSDKGameSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &URTSDKGameSettingScreen::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &URTSDKGameSettingScreen::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &URTSDKGameSettingScreen::HandleCancelChangesAction)));
}

UGameSettingRegistry* URTSDKGameSettingScreen::CreateRegistry()
{
	ULocalPlayer* player = GetOwningLocalPlayer();
	if (player == nullptr)
	{
		return nullptr;
	}
	URTSDKPlayerSettingsSubsystem* playersettingssubsystem = player->GetSubsystem<URTSDKPlayerSettingsSubsystem>();
	if (playersettingssubsystem == nullptr)
	{
		return nullptr;
	}
	return playersettingssubsystem->GetSettingsRegistry();
}

void URTSDKGameSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void URTSDKGameSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void URTSDKGameSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void URTSDKGameSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
