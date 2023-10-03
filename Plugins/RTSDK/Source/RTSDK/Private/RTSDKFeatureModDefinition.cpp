// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureModDefinition.h"
#include "RTSDKModManager.h"
#include "RTSDKGameFeatureData.h"
#include "GameFeaturesSubsystem.h"

FName URTSDKFeatureModDefinition::GetModType() const
{
	return RTSDKModTypeNames::Feature;
}
