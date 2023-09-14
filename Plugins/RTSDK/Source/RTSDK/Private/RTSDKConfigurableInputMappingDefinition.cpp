// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKConfigurableInputMappingDefinition.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

void URTSDKConfigurableInputMappingDefinition::Init(FName inDevName, const FText& inDisplayName, const FText& inDescriptionText, FName inAssociatedGameModDevName, TSoftObjectPtr<UInputMappingContext> inMappingContextAsset)
{
	MappingDevName = inDevName;
	MappingDisplayName = inDisplayName;
	MappingDescriptionText = inDescriptionText;
	AssociatedGameModDevName = inAssociatedGameModDevName;
	MappingContextAsset = inMappingContextAsset;
	bIsValid = false;
}

void URTSDKConfigurableInputMappingDefinition::BuildModDependencies(URTSDKModManager* inModManager)
{
	URTSDKGameModDefinition* gamemod = inModManager->GetGameModByName(AssociatedGameModDevName);
	if (gamemod != nullptr)
	{
		AssociatedGameMod = gamemod;
		bIsValid = true;
		return;
	}
	else
	{
		bIsValid = false;
		return;
	}
}

void URTSDKConfigurableInputMappingDefinition::BuildMod(URTSDKModManager* inModManager)
{
	if (!bIsValid)
	{
		return;
	}
	if (!MappingContextAsset.IsNull())
	{
		MappingContextData = FRTSDKSavedBindContext(MappingContextAsset.LoadSynchronous());
	}
	else
	{
		bIsValid = false;
		return;
	}
}
