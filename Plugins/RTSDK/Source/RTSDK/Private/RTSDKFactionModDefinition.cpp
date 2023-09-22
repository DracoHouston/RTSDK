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
//	bIsValid = false;
//	bIsFullyLoaded = false;
//	bIsActivated = false;
//	if (inData != nullptr)
//	{
//		ParentFactionModName = inData->ParentModDevName;
//		AssociatedGameModName = inData->AssociatedGameModDevName;
//		FactionClass = inData->FactionClass;
//		ModDevName = inData->DevName;
//		ModDisplayName = inData->DisplayName;
//		bIsAbstractMod = inData->bIsAbstractMod;
//		FeatureDependencyNames = inData->FeatureDependencies;
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

//void URTSDKFactionModDefinition::BuildModDependencies(URTSDKModManager* inModManager)
//{
//	if (!bIsValid)
//	{
//		return;
//	}
//
//	if (AssociatedGameModName.IsNone())
//	{
//		bIsValid = false;
//		return;
//	}
//	else
//	{
//		URTSDKGameModDefinition* gamemod = inModManager->GetGameModByName(AssociatedGameModName);
//		if ((gamemod != nullptr) && (gamemod->bIsValid))
//		{
//			AssociatedGameMod = gamemod;
//		}
//		else
//		{
//			bIsValid = false;
//			return;
//		}
//	}
//
//	if (!ParentFactionModName.IsNone())
//	{
//		URTSDKFactionModDefinition* parent = inModManager->GetFactionModByName(ParentFactionModName);
//		if ((parent != nullptr) && (parent->bIsValid))
//		{
//			ParentFactionMod = parent;
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

//void URTSDKFactionModDefinition::BuildMod(URTSDKModManager* inModManager)
//{
//	if (!bIsValid)
//	{
//		return;
//	}
//
//	TArray<URTSDKFactionModDefinition*> allparents;
//	URTSDKFactionModDefinition* currentouter = ParentFactionMod;
//	while (currentouter != nullptr)
//	{
//		if (!currentouter->bIsValid)
//		{
//			bIsValid = false;//bad parent
//			return;
//		}
//		if (allparents.Contains(currentouter))
//		{
//			bIsValid = false;//circular dependency
//			return;
//		}
//		allparents.Add(currentouter);
//		currentouter = currentouter->ParentFactionMod;
//	}
//}

//TArray<FString> URTSDKFactionModDefinition::LoadMod()
//{
//	TArray<FString> retval;
//
//	bIsFullyLoaded = false;
//
//	if (!ParentFactionModName.IsNone() && (ParentFactionMod != nullptr))
//	{
//		retval += ParentFactionMod->LoadMod();
//	}
//
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
