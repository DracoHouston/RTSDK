// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameModDefinition.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKFeatureModDefinition.h"
#include "RTSDKFactionModDefinition.h"
#include "RTSDKMapModDefinition.h"
#include "RTSDKConfigurableHUDDefinition.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

//void URTSDKGameModDefinition::BuildModDependencies(URTSDKModManager* inModManager)
//{
//	if (!bIsValid)
//	{
//		return;
//	}
//	
//	if (!ParentGameModName.IsNone())
//	{
//		URTSDKGameModDefinition* parent = inModManager->GetGameModByName(ParentGameModName);
//		if ((parent != nullptr) && (parent->bIsValid))
//		{
//			ParentGameMod = parent;
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

//void URTSDKGameModDefinition::Init(const URTSDKGameFeatureData* inData)
//{
//	ModTypeName = RTSDKModTypeNames::Game;
//	ParentModInfo.ModType = RTSDKModTypeNames::Game;
//	Super::Init(inData);
//}

TArray<IRTSDKDataEditorTreeNodeInterface*> URTSDKGameModDefinition::GetChildNodeObjectsForDataEditorTree()
{
	TArray<IRTSDKDataEditorTreeNodeInterface*> childnodes;
	for (int32 i = 0; i < ValidMapMods.Num(); i++)
	{
		IRTSDKDataEditorTreeNodeInterface* child = Cast<IRTSDKDataEditorTreeNodeInterface>(ValidMapMods[i]);
		if (child != nullptr)
		{
			childnodes.Add(child);
		}
	}
	for (int32 i = 0; i < ValidFactionMods.Num(); i++)
	{
		IRTSDKDataEditorTreeNodeInterface* child = Cast<IRTSDKDataEditorTreeNodeInterface>(ValidFactionMods[i]);
		if (child != nullptr)
		{
			childnodes.Add(child);
		}
	}

	childnodes += Super::GetChildNodeObjectsForDataEditorTree();

	return childnodes;
}

void URTSDKGameModDefinition::BuildMod(URTSDKModManager* inModManager)
{
	Super::BuildMod(inModManager);
	if (ModState == ERTSDKModStates::Invalid)
	{
		return;
	}

	ValidMapMods.Empty();
	ValidFactionMods.Empty();
	ValidMapMods += inModManager->GetMapModsByGameMod(this);
	ValidFactionMods += inModManager->GetFactionModsByGameMod(this);

	//	
	//TArray<URTSDKGameModDefinition*> allparents;
	//URTSDKGameModDefinition* currentouter = ParentGameMod;
	//while (currentouter != nullptr)
	//{
	//	if (!currentouter->bIsValid)
	//	{
	//		bIsValid = false;//bad parent
	//		return;
	//	}
	//	if (allparents.Contains(currentouter))
	//	{
	//		bIsValid = false;//circular dependency
	//		return;
	//	}
	//	allparents.Add(currentouter);
	//	currentouter = currentouter->ParentGameMod;
	//}

	//ValidMapMods.Empty();
	//ValidFactionMods.Empty();
	//ValidConfigurableHUDs.Empty();
	//ValidMapMods += inModManager->GetMapModsByGameMod(this);
	//ValidFactionMods += inModManager->GetFactionModsByGameMod(this);
	//ValidConfigurableHUDs += inModManager->GetConfigurableHUDsByGameMod(this);
	//for (int32 i = 0; i < allparents.Num(); i++)
	//{
	//	ValidMapMods += inModManager->GetMapModsByGameMod(allparents[i]);
	//	ValidFactionMods += inModManager->GetFactionModsByGameMod(allparents[i]);
	//	ValidConfigurableHUDs += inModManager->GetConfigurableHUDsByGameMod(allparents[i]);
	//}
}

FName URTSDKGameModDefinition::GetModType() const
{
	return RTSDKModTypeNames::Game;
}

//TArray<FString> URTSDKGameModDefinition::LoadMod()
//{
//	TArray<FString> retval;
//
//	bIsFullyLoaded = false;
//
//	if (!ParentGameModName.IsNone() && (ParentGameMod != nullptr))
//	{
//		retval += ParentGameMod->LoadMod();
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
