// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKGameSettingListEntry.h"
#include "RTSDKKeybindSettingListEntry.generated.h"


class URTSDKKeybindSettingsButton;

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKKeybindSettingListEntry : public URTSDKGameSettingListEntry
{
	GENERATED_BODY()

public:

	virtual void SetSetting(UGameSetting* InSetting) override;

protected:

		//Required bound widget for the primary bind button, derived from URTSDKKeybindSettingsButton
		UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<URTSDKKeybindSettingsButton> PrimaryBindButton;

	//Required bound widget for the secondary bind button, derived from URTSDKKeybindSettingsButton
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<URTSDKKeybindSettingsButton> SecondaryBindButton;
};
