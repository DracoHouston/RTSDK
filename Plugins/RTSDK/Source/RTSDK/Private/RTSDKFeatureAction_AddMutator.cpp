// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddMutator.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKMutatorDefinition.h"
#include "RTSDKModManager.h"

void URTSDKFeatureAction_AddMutator::OnGameFeatureRegistering()
{
	/*URTSDKModManager * modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	URTSDKMutatorDefinition* mod = NewObject<URTSDKMutatorDefinition>(modmanager);
	mod->Init(ModDevName, ModDisplayName, GameFeatureName, AdditionalGameFeaturesToEnable, AssociatedGameModDevName);
	modmanager->RegisterMutator(mod);*/
}

void URTSDKFeatureAction_AddMutator::OnGameFeatureUnregistering()
{
}
