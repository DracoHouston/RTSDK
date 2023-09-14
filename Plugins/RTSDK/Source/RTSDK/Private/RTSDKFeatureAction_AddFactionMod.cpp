// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddFactionMod.h"
#include "RTSDKFactionModDefinition.h"
#include "RTSDKFactionDefinition.h"
#include "RTSDKModManager.h"

void URTSDKFeatureAction_AddFactionMod::OnGameFeatureRegistering()
{
	/*URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	URTSDKFactionModDefinition* mod = NewObject<URTSDKFactionModDefinition>(modmanager);
	mod->Init(ModDevName, ModDisplayName, GameFeatureName, FactionClass, AssociatedGameModDevName, bIsAbstractMod, ParentFactionModDevName);
	modmanager->RegisterFactionMod(mod);*/
}

void URTSDKFeatureAction_AddFactionMod::OnGameFeatureUnregistering()
{
}
