// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddHUDToSettings.h"
#include "RTSDKModManager.h"
#include "RTSDKHUDLayoutData.h"
#include "RTSDKConfigurableHUDDefinition.h"

void URTSDKFeatureAction_AddHUDToSettings::OnGameFeatureRegistering()
{
	/*if (HUDLayoutData.IsNull())
	{
		return;
	}
	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	URTSDKHUDLayoutData* layoutdata = HUDLayoutData.LoadSynchronous();
	URTSDKConfigurableHUDDefinition* newhud = NewObject<URTSDKConfigurableHUDDefinition>(modmanager);
	newhud->Init(
		HUDDevName, 
		HUDDisplayName, 
		HUDDescriptionText, 
		AssociatedGameModDevName, 
		ParentHUDDevName,
		layoutdata->WidgetClass,
		layoutdata->Elements,
		bIsAbstractHUD);
	modmanager->RegisterConfigurableHUD(newhud);*/
}

void URTSDKFeatureAction_AddHUDToSettings::OnGameFeatureUnregistering()
{
}
