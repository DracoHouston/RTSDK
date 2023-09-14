// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKMapModDefinition.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKFeatureModDefinition.h"
#include "Engine/Level.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

void URTSDKMapModDefinition::Init(const URTSDKGameFeatureData* inData)
{
	bIsValid = false;
	bIsFullyLoaded = false;
	bIsActivated = false;
	if (inData != nullptr)
	{
		ParentMapModName = inData->ParentModDevName;
		AssociatedGameModName = inData->AssociatedGameModDevName;
		ModDevName = inData->DevName;
		ModDisplayName = inData->DisplayName;
		bIsAbstractMod = inData->bIsAbstractMod;
		FeatureDependencyNames = inData->FeatureDependencies;
		AssociatedLevel = inData->MapLevel;
		GameFeatureName = inData->GetName();
		UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
		FString featureurl;
		if (gamefeatures.GetPluginURLByName(GameFeatureName, featureurl))
		{
			GameFeatureURL = featureurl;
			bIsValid = true;
			return;
		}
		else
		{
			//bad game feature name
			bIsValid = false;
			return;
		}
	}
	else
	{
		//bad game feature data
		bIsValid = false;
		return;
	}
}

void URTSDKMapModDefinition::BuildModDependencies(URTSDKModManager* inModManager)
{
	if (!bIsValid)
	{
		return;
	}

	if (AssociatedGameModName.IsNone())
	{
		bIsValid = false;
		return;
	}
	else
	{
		URTSDKGameModDefinition* gamemod = inModManager->GetGameModByName(AssociatedGameModName);
		if ((gamemod != nullptr) && (gamemod->bIsValid))
		{
			AssociatedGameMod = gamemod;
		}
		else
		{
			bIsValid = false;
			return;
		}
	}

	if (!ParentMapModName.IsNone())
	{
		URTSDKMapModDefinition* parent = inModManager->GetMapModByName(ParentMapModName);
		if ((parent != nullptr) && (parent->bIsValid))
		{
			ParentMapMod = parent;
		}
	}

	FeatureDependencies.Empty(FeatureDependencyNames.Num());
	for (int32 i = 0; i < FeatureDependencyNames.Num(); i++)
	{
		URTSDKFeatureModDefinition* feature = inModManager->GetFeatureModByName(FeatureDependencyNames[i]);
		if ((feature != nullptr) && (feature->bIsValid))
		{
			FeatureDependencies.Add(feature);
		}
		else
		{
			//missing feature mod dependency
			bIsValid = false;
			return;
		}
	}
}

void URTSDKMapModDefinition::BuildMod(URTSDKModManager* inModManager)
{
	if (!bIsValid)
	{
		return;
	}

	TArray<URTSDKMapModDefinition*> allparents;
	URTSDKMapModDefinition* currentouter = ParentMapMod;
	while (currentouter != nullptr)
	{
		if (!currentouter->bIsValid)
		{
			bIsValid = false;//bad parent
			return;
		}
		if (allparents.Contains(currentouter))
		{
			bIsValid = false;//circular dependency
			return;
		}
		allparents.Add(currentouter);
		currentouter = currentouter->ParentMapMod;
	}
}

TArray<FString> URTSDKMapModDefinition::LoadMod()
{
	TArray<FString> retval;

	bIsFullyLoaded = false;

	if (!ParentMapModName.IsNone() && (ParentMapMod != nullptr))
	{
		retval += ParentMapMod->LoadMod();
	}


	for (int32 i = 0; i < FeatureDependencies.Num(); i++)
	{
		retval += FeatureDependencies[i]->LoadMod();
	}

	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();

	gamefeatures.LoadGameFeaturePlugin(GameFeatureURL, FGameFeaturePluginLoadComplete::CreateLambda(
		[this](const UE::GameFeatures::FResult& Result)
		{
			bIsFullyLoaded = true;
		}));
	retval.Add(GameFeatureURL);

	return retval;
}
