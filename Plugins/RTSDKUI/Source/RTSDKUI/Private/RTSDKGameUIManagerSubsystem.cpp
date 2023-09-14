// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameUIManagerSubsystem.h"
#include "RTSDKUIDeveloperSettings.h"

void URTSDKGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (GetCurrentUIPolicy() == nullptr)
	{
		URTSDKUIDeveloperSettings* uidevsettings = GetMutableDefault<URTSDKUIDeveloperSettings>();
		if (uidevsettings->DefaultUIPolicyClass != nullptr)
		{
			SwitchToPolicy(NewObject<UGameUIPolicy>(this, uidevsettings->DefaultUIPolicyClass));
		}
	}
	Super::Initialize(Collection);
}
