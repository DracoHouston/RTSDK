// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureModDefinition.h"
#include "RTSDKModManager.h"
#include "RTSDKGameFeatureData.h"
#include "GameFeaturesSubsystem.h"

//void URTSDKFeatureModDefinition::Init(const URTSDKGameFeatureData* inData)
//{
//	bIsValid = false;
//	bIsFullyLoaded = false;
//	bIsActivated = false;
//	if (inData != nullptr)
//	{
//		DevName = inData->DevName;
//		DisplayName = inData->DisplayName;
//		GameFeatureName = inData->GetName();
//		UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
//		FString featureurl;
//		if (gamefeatures.GetPluginURLByName(GameFeatureName, featureurl))
//		{
//			GameFeatureURL = featureurl;
//			bIsValid = true;
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

//TArray<FString> URTSDKFeatureModDefinition::LoadMod()
//{
//	TArray<FString> retval;
//
//	bIsFullyLoaded = false;	
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
