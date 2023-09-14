// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKModUserSettingDefinition.h"

void URTSDKModUserSettingDefinition::Init(FName inDevName, FText inDisplayName, FString inGameFeatureName)
{
	DevName = inDevName;
	DisplayName = inDisplayName;
	GameFeatureName = inGameFeatureName;
}
