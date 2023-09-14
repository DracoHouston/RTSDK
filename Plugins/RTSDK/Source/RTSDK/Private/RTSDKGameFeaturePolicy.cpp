// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKGameFeaturePolicy.h"
#include "RTSDKModManager.h"

void URTSDKGameFeaturePolicy::InitGameFeatureManager()
{
	Super::InitGameFeatureManager();
	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	modmanager->BuildMods();
}
