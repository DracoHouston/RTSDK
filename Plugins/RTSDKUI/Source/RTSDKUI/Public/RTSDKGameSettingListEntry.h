// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameSettingListEntry.h"
#include "RTSDKGameSettingListEntry.generated.h"

class UCommonRichTextBlock;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta = (Category = "Settings", DisableNativeTick))
class RTSDKUI_API URTSDKGameSettingListEntry : public UGameSettingListEntryBase
{
	GENERATED_BODY()

public:

	virtual void SetSetting(UGameSetting* InSetting) override;
	virtual void SetDisplayNameOverride(const FText& OverrideName) override;

protected:

	//Required bound widget for the display name, Common Rich Text Block
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCommonRichTextBlock> SettingsDisplayNameText;
};
