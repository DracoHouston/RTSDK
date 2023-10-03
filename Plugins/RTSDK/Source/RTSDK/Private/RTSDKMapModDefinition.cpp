// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKMapModDefinition.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKFeatureModDefinition.h"
#include "Engine/Level.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

//void URTSDKMapModDefinition::Init(const URTSDKGameFeatureData* inData)
//{
//	ModTypeName = RTSDKModTypeNames::Map;
//	ParentModInfo.ModType = RTSDKModTypeNames::Map;
//	AssociatedModInfo.ModType = RTSDKModTypeNames::Game;
//	Super::Init(inData);
//	AssociatedLevel = inData->MapLevel;
//}

FName URTSDKMapModDefinition::GetModType() const
{
	return RTSDKModTypeNames::Map;
}

FName URTSDKMapModDefinition::GetAllowedAssociatedModType() const
{
	return RTSDKModTypeNames::Game;
}
