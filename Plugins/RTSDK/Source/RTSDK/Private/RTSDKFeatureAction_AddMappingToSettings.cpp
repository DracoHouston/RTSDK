// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddMappingToSettings.h"
#include "InputMappingContext.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKConfigurableInputMappingDefinition.h"
#include "RTSDKModManager.h"
#include "RTSDKConfigurableHUDDefinition.h"

void URTSDKFeatureAction_AddMappingToSettings::OnGameFeatureRegistering()
{
	/*URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	
	URTSDKConfigurableInputMappingDefinition* mapping = NewObject<URTSDKConfigurableInputMappingDefinition>(modmanager);
	mapping->Init(
		MappingDevName, 
		MappingDisplayName, 
		MappingDescriptionText, 
		AssociatedGameModDevName, 
		MappingContextAsset);
	modmanager->RegisterConfigurableInputMapping(mapping);*/
}

void URTSDKFeatureAction_AddMappingToSettings::OnGameFeatureUnregistering()
{
}
