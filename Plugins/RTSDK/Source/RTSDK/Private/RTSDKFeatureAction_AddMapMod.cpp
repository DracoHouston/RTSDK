// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddMapMod.h"
#include "RTSDKMapModDefinition.h"
#include "Engine/Level.h"
#include "RTSDKModManager.h"

void URTSDKFeatureAction_AddMapMod::OnGameFeatureRegistering()
{
	/*URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	URTSDKMapModDefinition* mod = NewObject<URTSDKMapModDefinition>(modmanager);
	mod->Init(ModDevName, ModDisplayName, GameFeatureName, AssociatedLevel, AssociatedGameModDevName, bIsAbstractMod, ParentMapModDevName);
	modmanager->RegisterMapMod(mod);*/
}

void URTSDKFeatureAction_AddMapMod::OnGameFeatureUnregistering()
{
}
