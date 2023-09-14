// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddGameMod.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKModManager.h"

void URTSDKFeatureAction_AddGameMod::OnGameFeatureRegistering()
{
	/*URTSDKModManager * modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	URTSDKGameModDefinition* mod = NewObject<URTSDKGameModDefinition>(modmanager);
	mod->Init(ModDevName, ModDisplayName, GameFeatureName, bIsAbstractMod, ParentGameModDevName);
	modmanager->RegisterGameMod(mod);*/
}

void URTSDKFeatureAction_AddGameMod::OnGameFeatureUnregistering()
{
}
