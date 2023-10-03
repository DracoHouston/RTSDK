// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFactionModDefinition.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKFeatureModDefinition.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKFactionDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

//void URTSDKFactionModDefinition::Init(const URTSDKGameFeatureData* inData)
//{
//	ModTypeName = RTSDKModTypeNames::Faction;
//	ParentModInfo.ModType = RTSDKModTypeNames::Faction;
//	AssociatedModInfo.ModType = RTSDKModTypeNames::Game;
//	Super::Init(inData);
//}

FName URTSDKFactionModDefinition::GetModType() const
{
	return RTSDKModTypeNames::Faction;
}

FName URTSDKFactionModDefinition::GetAllowedAssociatedModType() const
{
	return RTSDKModTypeNames::Game;
}
