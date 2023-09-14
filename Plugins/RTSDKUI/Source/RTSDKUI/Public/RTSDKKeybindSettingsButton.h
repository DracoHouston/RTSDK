// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKTypes.h"
#include "GameSettingFilterState.h"
#include "RTSDKKeybindSettingsButton.generated.h"

class UGameSetting;
class URTSDKButton;
class UCommonRichTextBlock;
class URTSDKGameSettingKeybind;

/**
 * Blueprintable UMG Slate Widget with some native variables and methods to drive a keybind settings editor button in the RTSDK Game Setting Screen
 * Designed to be a button etc that displays the bound keys in some form. These are generally managed by a bp class derived from RTSDKKeybindSettingListEntry
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKKeybindSettingsButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void SetupKeybind(URTSDKGameSettingKeybind* inBindData, bool inIsPrimaryBind);

protected:

	//Required bound widget for the inner button widget, URTSDKButton
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<URTSDKButton> BindButton;

	//Required bound widget for the currently bound keys for this bind, UCommonRichTextBlock, inline icon support.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCommonRichTextBlock> BoundKeysRichText;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<URTSDKGameSettingKeybind> BindData;

	UPROPERTY(BlueprintReadWrite)
		bool bPrimaryBind;	

		void OnLeftClicked();
		void OnRightClicked();
		void OnMiddleClicked();
		void OnSettingChanged(UGameSetting* InSetting, EGameSettingChangeReason InChangeReason);
};
