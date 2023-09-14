// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKUIDeveloperSettings.h"
#include "GameUIPolicy.h"
#include "CommonActivatableWidget.h"

URTSDKUIDeveloperSettings::URTSDKUIDeveloperSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultUIPolicyClass = nullptr;
	GameMenuWidgetClass = nullptr;
	MainMenuWidgetClass = nullptr;
}
