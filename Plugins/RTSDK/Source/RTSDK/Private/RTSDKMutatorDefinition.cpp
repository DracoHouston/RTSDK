// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKMutatorDefinition.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKFeatureModDefinition.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

//void URTSDKMutatorDefinition::Init(const URTSDKGameFeatureData* inData)
//{
//	bIsValid = false;
//	bIsFullyLoaded = false;
//	bIsActivated = false;
//	if (inData != nullptr)
//	{
//		ModDevName = inData->DevName;
//		ModDisplayName = inData->DisplayName;
//		FeatureDependencyNames = inData->FeatureDependencies;
//		AssociatedGameModName = inData->AssociatedGameModDevName;
//		GameFeatureName = inData->GetName();
//		UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
//		FString featureurl;
//		if (gamefeatures.GetPluginURLByName(GameFeatureName, featureurl))
//		{
//			GameFeatureURL = featureurl;
//			bIsValid = true;
//			return;
//		}
//		else
//		{
//			//bad game feature name
//			bIsValid = false;
//			return;
//		}
//	}
//	else
//	{
//		//bad game feature data
//		bIsValid = false;
//		return;
//	}
//}
//
//void URTSDKMutatorDefinition::BuildModDependencies(URTSDKModManager* inModManager)
//{
//	if (GameFeatureName.Len() <= 0)
//	{
//		bIsValid = false;
//		return;
//	}
//
//	if (AssociatedGameModName.IsNone())
//	{
//		bIsValid = true;
//		return;
//	}
//	else
//	{
//		URTSDKGameModDefinition* associatedgamemod = inModManager->GetGameModByName(AssociatedGameModName);
//		if (associatedgamemod != nullptr)
//		{
//			AssociatedGameMod = associatedgamemod;
//			bIsValid = true;
//			return;
//		}
//		else
//		{
//			bIsValid = false;
//			return;
//		}
//	}
//
//	FeatureDependencies.Empty(FeatureDependencyNames.Num());
//	for (int32 i = 0; i < FeatureDependencyNames.Num(); i++)
//	{
//		URTSDKFeatureModDefinition* feature = inModManager->GetFeatureModByName(FeatureDependencyNames[i]);
//		if ((feature != nullptr) && (feature->bIsValid))
//		{
//			FeatureDependencies.Add(feature);
//		}
//		else
//		{
//			//missing feature mod dependency
//			bIsValid = false;
//			return;
//		}
//	}
//}
//
//void URTSDKMutatorDefinition::BuildMod(URTSDKModManager* inModManager)
//{
//}
//
//TArray<FString> URTSDKMutatorDefinition::LoadMod()
//{
//	TArray<FString> retval;
//
//	bIsFullyLoaded = false;
//
//	for (int32 i = 0; i < FeatureDependencies.Num(); i++)
//	{
//		retval += FeatureDependencies[i]->LoadMod();
//	}
//
//	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
//
//	gamefeatures.LoadGameFeaturePlugin(GameFeatureURL, FGameFeaturePluginLoadComplete::CreateLambda(
//		[this](const UE::GameFeatures::FResult& Result)
//		{
//			bIsFullyLoaded = true;
//		}));
//	retval.Add(GameFeatureURL);
//
//	return retval;
//}

void URTSDKMutatorDefinition::Init(const URTSDKGameFeatureData* inData)
{
	AssociatedModInfo.ModType = RTSDKModTypeNames::Game;
	Super::Init(inData);
}
