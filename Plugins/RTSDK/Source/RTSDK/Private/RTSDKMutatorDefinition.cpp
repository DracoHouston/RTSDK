// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKMutatorDefinition.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKFeatureModDefinition.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

//void URTSDKMutatorDefinition::Init(const URTSDKGameFeatureData* inData)
//{
//	ModTypeName = RTSDKModTypeNames::Mutator;
//	ParentModInfo.ModType = RTSDKModTypeNames::Mutator;
//	AssociatedModInfo.ModType = RTSDKModTypeNames::Game;
//	Super::Init(inData);
//}

FName URTSDKMutatorDefinition::GetModType() const
{
	return RTSDKModTypeNames::Mutator;
}

FName URTSDKMutatorDefinition::GetAllowedAssociatedModType() const
{
	return RTSDKModTypeNames::Game;
}
