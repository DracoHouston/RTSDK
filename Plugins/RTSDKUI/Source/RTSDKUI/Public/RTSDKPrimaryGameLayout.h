// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryGameLayout.h"
#include "RTSDKPrimaryGameLayout.generated.h"

class UCommonActivatableWidgetStack;
class UCommonActivatableWidgetQueue;
class URTSDKHUDPanel;

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKPrimaryGameLayout : public UPrimaryGameLayout
{
	GENERATED_BODY()
	
public:
	//Mandatory RTSDK HUD Panel, host for RTSDK HUD Element Panel widgets as per developer and user settings, should be lowest layer
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetStack> Layer_HUD;

	//Mandatory Common Activatable Widget Stack to be host to the ingame menu, should be above HUD layer.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetStack> Layer_GameMenu;

	//Mandatory Common Activatable Widget Stack to be host to the main menu, should be above Game Menu layer
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetStack> Layer_Menu;

	//Mandatory Common Activatable Widget Queue to be host to modal dialogues, should be above Main Menu layer
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetQueue> Layer_Modal;
};
