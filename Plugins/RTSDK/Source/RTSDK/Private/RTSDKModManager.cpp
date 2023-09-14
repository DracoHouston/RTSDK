// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKModManager.h"
#include "RTSDKFeatureModDefinition.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKFactionModDefinition.h"
#include "RTSDKMapModDefinition.h" 
#include "RTSDKMutatorDefinition.h"
#include "RTSDKConfigurableHUDDefinition.h"
#include "RTSDKConfigurableInputMappingDefinition.h"
#include "GameFeaturesSubsystem.h"
#include "RTSDKGameFeatureData.h"

void URTSDKModManager::BuildMods()
{
	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
	gamefeatures.ForEachRegisteredGameFeature<URTSDKGameFeatureData>(
		[this](const URTSDKGameFeatureData* inData)
		{
			if (inData->GameFeatureType == ERTSDKGameFeatureTypes::Feature)
			{
				URTSDKFeatureModDefinition* mod = NewObject<URTSDKFeatureModDefinition>(this);
				mod->Init(inData);
				this->RegisterFeatureMod(mod);
			}
			else if (inData->GameFeatureType == ERTSDKGameFeatureTypes::Mutator)
			{
				URTSDKMutatorDefinition* mod = NewObject<URTSDKMutatorDefinition>(this);
				mod->Init(inData);
				this->RegisterMutator(mod);
			}
			else if (inData->GameFeatureType == ERTSDKGameFeatureTypes::Game)
			{
				URTSDKGameModDefinition* mod = NewObject<URTSDKGameModDefinition>(this);
				mod->Init(inData);
				this->RegisterGameMod(mod);
			}
			else if (inData->GameFeatureType == ERTSDKGameFeatureTypes::Faction)
			{
				URTSDKFactionModDefinition* mod = NewObject<URTSDKFactionModDefinition>(this);
				mod->Init(inData);
				this->RegisterFactionMod(mod);
			}
			else if (inData->GameFeatureType == ERTSDKGameFeatureTypes::Map)
			{
				URTSDKMapModDefinition* mod = NewObject<URTSDKMapModDefinition>(this);
				mod->Init(inData);
				this->RegisterMapMod(mod);
			}			
		});

	for (auto It = GameModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = FactionModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = MapModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = MutatorsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = ConfigurableHUDByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = FactionModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	for (auto It = MapModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	for (auto It = MutatorsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	for (auto It = ConfigurableHUDByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	for (auto It = GameModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}
}

void URTSDKModManager::RegisterFeatureMod(URTSDKFeatureModDefinition* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	if (inModDef->DevName.IsNone())
	{
		return;
	}
	if (!inModDef->bIsValid)
	{
		return;
	}
	FeatureModsByName.Add(inModDef->DevName, inModDef);
}

void URTSDKModManager::RegisterGameMod(URTSDKGameModDefinition* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	if (inModDef->ModDevName.IsNone())
	{
		return;
	}
	if (!inModDef->bIsValid)
	{
		return;
	}
	GameModsByName.Add(inModDef->ModDevName, inModDef);
}

void URTSDKModManager::RegisterFactionMod(URTSDKFactionModDefinition* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	if (inModDef->ModDevName.IsNone())
	{
		return;
	}
	if (!inModDef->bIsValid)
	{
		return;
	}
	FactionModsByName.Add(inModDef->ModDevName, inModDef);
}

void URTSDKModManager::RegisterMapMod(URTSDKMapModDefinition* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	if (inModDef->ModDevName.IsNone())
	{
		return;
	}
	if (!inModDef->bIsValid)
	{
		return;
	}
	MapModsByName.Add(inModDef->ModDevName, inModDef);
}

void URTSDKModManager::RegisterMutator(URTSDKMutatorDefinition* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	if (inModDef->ModDevName.IsNone())
	{
		return;
	}
	if (!inModDef->bIsValid)
	{
		return;
	}
	MutatorsByName.Add(inModDef->ModDevName, inModDef);
}

void URTSDKModManager::RegisterConfigurableHUD(URTSDKConfigurableHUDDefinition* inHUDDef)
{
	if (inHUDDef == nullptr)
	{
		return;
	}
	if (inHUDDef->HUDDevName.IsNone())
	{
		return;
	}
	ConfigurableHUDByName.Add(inHUDDef->HUDDevName, inHUDDef);
}

void URTSDKModManager::RegisterConfigurableInputMapping(URTSDKConfigurableInputMappingDefinition* inMappingDef)
{
	if (inMappingDef == nullptr)
	{
		return;
	}
	if (inMappingDef->MappingDevName.IsNone())
	{
		return;
	}
	ConfigurableMappingsByName.Add(inMappingDef->MappingDevName, inMappingDef);
}

URTSDKFeatureModDefinition* URTSDKModManager::GetFeatureModByName(FName inModDevName) const
{
	if (FeatureModsByName.Contains(inModDevName))
	{
		return FeatureModsByName[inModDevName];
	}
	return nullptr;
}

URTSDKGameModDefinition* URTSDKModManager::GetGameModByName(FName inModDevName) const
{
	if (GameModsByName.Contains(inModDevName))
	{
		return GameModsByName[inModDevName];
	}
	return nullptr;
}

URTSDKFactionModDefinition* URTSDKModManager::GetFactionModByName(FName inModDevName) const
{
	if (FactionModsByName.Contains(inModDevName))
	{
		return FactionModsByName[inModDevName];
	}
	return nullptr;
}

URTSDKMapModDefinition* URTSDKModManager::GetMapModByName(FName inModDevName) const
{
	if (MapModsByName.Contains(inModDevName))
	{
		return MapModsByName[inModDevName];
	}
	return nullptr;
}

URTSDKMutatorDefinition* URTSDKModManager::GetMutatorByName(FName inModDevName) const
{
	if (MutatorsByName.Contains(inModDevName))
	{
		return MutatorsByName[inModDevName];
	}
	return nullptr;
}

URTSDKConfigurableHUDDefinition* URTSDKModManager::GetConfigurableHUDByName(FName inHUDDevName) const
{
	if (ConfigurableHUDByName.Contains(inHUDDevName))
	{
		return ConfigurableHUDByName[inHUDDevName];
	}
	return nullptr;
}

URTSDKConfigurableInputMappingDefinition* URTSDKModManager::GetConfigurableInputMappingByName(FName inMappingDevName) const
{
	if (ConfigurableMappingsByName.Contains(inMappingDevName))
	{
		return ConfigurableMappingsByName[inMappingDevName];
	}
	return nullptr;
}

TArray<URTSDKFactionModDefinition*> URTSDKModManager::GetFactionModsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	TArray<URTSDKFactionModDefinition*> factions;
	for (auto It = FactionModsByName.CreateConstIterator(); It; ++It)
	{
		if ((It->Value->bIsValid) && (!It->Value->bIsAbstractMod) && (It->Value->AssociatedGameMod == inGameMod))
		{
			factions.Add(It->Value);
		}
	}
	return factions;
}

TArray<URTSDKMapModDefinition*> URTSDKModManager::GetMapModsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	TArray<URTSDKMapModDefinition*> maps;
	for (auto It = MapModsByName.CreateConstIterator(); It; ++It)
	{
		if ((It->Value->bIsValid) && (!It->Value->bIsAbstractMod) && (It->Value->AssociatedGameMod == inGameMod))
		{
			maps.Add(It->Value);
		}
	}
	return maps;
}

TArray<URTSDKMutatorDefinition*> URTSDKModManager::GetMutatorsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	TArray<URTSDKMutatorDefinition*> mutators;
	for (auto It = MutatorsByName.CreateConstIterator(); It; ++It)
	{
		if ((It->Value->bIsValid) && ((It->Value->AssociatedGameMod == inGameMod) || (It->Value->AssociatedGameModName.IsNone())))
		{
			mutators.Add(It->Value);
		}
	}
	return mutators;
}

TArray<URTSDKConfigurableHUDDefinition*> URTSDKModManager::GetConfigurableHUDsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	TArray<URTSDKConfigurableHUDDefinition*> huds;
	for (auto It = ConfigurableHUDByName.CreateConstIterator(); It; ++It)
	{
		if ((It->Value->bIsValid) && (!It->Value->bIsAbstractHUD) && (It->Value->AssociatedGameMod == inGameMod))
		{
			huds.Add(It->Value);
		}
	}
	return huds;
}

TArray<URTSDKConfigurableInputMappingDefinition*> URTSDKModManager::GetConfigurableMappingsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	TArray<URTSDKConfigurableInputMappingDefinition*> mappings;
	for (auto It = ConfigurableMappingsByName.CreateConstIterator(); It; ++It)
	{
		if ((It->Value->bIsValid) && (It->Value->AssociatedGameMod == inGameMod))
		{
			mappings.Add(It->Value);
		}
	}
	return mappings;
}

TArray<FString> URTSDKModManager::GetAllGameFeaturesForMods(URTSDKGameModDefinition* inGameMod, TArray<URTSDKFactionModDefinition*> inFactionMods, URTSDKMapModDefinition* inMapMod, TArray<URTSDKMutatorDefinition*> inMutators) const
{
	/*TArray<FString> retval = inGameMod->CombinedGameFeatureURLs;
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		for (int32 j = 0; j < inFactionMods[i]->CombinedGameFeatureURLs.Num(); j++)
		{
			retval.AddUnique(inFactionMods[i]->CombinedGameFeatureURLs[j]);
		}
	}
	for (int32 i = 0; i < inMapMod->CombinedGameFeatureURLs.Num(); i++)
	{
		retval.AddUnique(inMapMod->CombinedGameFeatureURLs[i]);
	}
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		for (int32 j = 0; j < inMutators[i]->CombinedGameFeatureURLs.Num(); j++)
		{
			retval.AddUnique(inMutators[i]->CombinedGameFeatureURLs[j]);
		}
	}*/

	TArray<FString> retval;



	return retval;
}

TArray<FString> URTSDKModManager::GetAllGameFeaturesForModsByName(FName inGameMod, TArray<FName> inFactionMods, FName inMapMod, TArray<FName> inMutators) const
{
	URTSDKGameModDefinition* gamemod = nullptr;
	if (GameModsByName.Contains(inGameMod))
	{
		gamemod = GameModsByName[inGameMod];
	}
	else
	{
		return TArray<FString>();
	}
	TArray<URTSDKFactionModDefinition*> factionmods;
	factionmods.Empty(inFactionMods.Num());
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		if (FactionModsByName.Contains(inFactionMods[i]))
		{
			factionmods.Add(FactionModsByName[inFactionMods[i]]);
		}
		else
		{
			return TArray<FString>();
		}
	}
	URTSDKMapModDefinition* mapmod = nullptr;
	if (MapModsByName.Contains(inMapMod))
	{
		mapmod = MapModsByName[inMapMod];
	}
	else
	{
		return TArray<FString>();
	}

	TArray<URTSDKMutatorDefinition*> mutators;
	mutators.Empty(inMutators.Num());
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		if (MutatorsByName.Contains(inMutators[i]))
		{
			mutators.Add(MutatorsByName[inMutators[i]]);
		}
		else
		{
			return TArray<FString>();
		}
	}
	return GetAllGameFeaturesForMods(gamemod, factionmods, mapmod, mutators);
}

FRTSDKActiveModsInfo URTSDKModManager::ActivateMods(URTSDKGameModDefinition* inGameMod, TArray<URTSDKFactionModDefinition*> inFactionMods, URTSDKMapModDefinition* inMapMod, TArray<URTSDKMutatorDefinition*> inMutators)
{
	if (inGameMod == nullptr)
	{
		return;//bad game mod input
	}
	if (!inGameMod->bIsValid)
	{
		return;//bad game mod, invalid
	}
	if (inMapMod == nullptr)
	{
		return;//bad map mod input
	}
	if (!inMapMod->bIsValid)
	{
		return;//bad game mod, invalid
	}
	if (inFactionMods.Num() == 0)
	{
		return;//no faction mods, even a game with 1 faction needs a faction defined!
	}
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		if (inFactionMods[i] == nullptr)
		{
			return;//bad faction mod input
		}
		if (!inFactionMods[i]->bIsValid)
		{
			return;//bad faction mod, invalid
		}
	}

	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		if (inMutators[i] == nullptr)
		{
			return;//bad mutator mod input
		}
		if (!inMutators[i]->bIsValid)
		{
			return;//bad mutator mod, invalid
		}
	}

	//todo: change this to call into a new 'activate mod' function that chains recursively through parents activating in order
	//order is game, factions, map, mutators, eldest parent first through to the active mod last
	//the mods must call activate on each of their feature dependencies then themselves, individually
	//we need to get the total number of feature plugins being activated to track when they all finish via callback
	FRTSDKActiveModsInfo retval;
	retval.ActiveGameMod = inGameMod;
	retval.ActiveFactionMods = inFactionMods;
	retval.ActiveMapMod = inMapMod;
	retval.ActiveMutators = inMutators;
	
	retval.MapLevelAsset = inMapMod->AssociatedLevel;
	if (inMapMod->GameMenuWidgetClass != nullptr)
	{
		retval.GameMenuOverrideClass = inMapMod->GameMenuWidgetClass;
	}
	else if (inGameMod->GameMenuWidgetClass != nullptr)
	{
		retval.GameMenuOverrideClass = inGameMod->GameMenuWidgetClass;
	}

	retval.GameFeatureURLs += inGameMod->LoadMod();
	retval.GameFeatureURLs += inMapMod->LoadMod();
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		retval.GameFeatureURLs += inFactionMods[i]->LoadMod();
	}
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		retval.GameFeatureURLs += inMutators[i]->LoadMod();
	}

	/*UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
	PendingGameFeatureCount = retval.GameFeatureURLs.Num();
	for (int32 i = 0; i < retval.GameFeatureURLs.Num(); i++)
	{
		gamefeatures.LoadAndActivateGameFeaturePlugin(retval.GameFeatureURLs[i], FGameFeaturePluginLoadComplete::CreateUObject(this, &URTSDKModManager::OnGameFeaturePluginLoadComplete));
	}*/
	
	ActiveMods = retval;
	return retval;
}

FRTSDKActiveModsInfo URTSDKModManager::ActivateModsByName(FName inGameMod, TArray<FName> inFactionMods, FName inMapMod, TArray<FName> inMutators)
{
	URTSDKGameModDefinition* gamemod = nullptr;
	if (GameModsByName.Contains(inGameMod))
	{
		gamemod = GameModsByName[inGameMod];
	}
	else
	{
		return FRTSDKActiveModsInfo();
	}
	TArray<URTSDKFactionModDefinition*> factionmods;
	factionmods.Empty(inFactionMods.Num());
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		if (FactionModsByName.Contains(inFactionMods[i]))
		{
			factionmods.Add(FactionModsByName[inFactionMods[i]]);
		}
		else
		{
			return FRTSDKActiveModsInfo();
		}
	}
	URTSDKMapModDefinition* mapmod = nullptr;
	if (MapModsByName.Contains(inMapMod))
	{
		mapmod = MapModsByName[inMapMod];
	}
	else
	{
		return FRTSDKActiveModsInfo();
	}

	TArray<URTSDKMutatorDefinition*> mutators;
	mutators.Empty(inMutators.Num());
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		if (MutatorsByName.Contains(inMutators[i]))
		{
			mutators.Add(MutatorsByName[inMutators[i]]);
		}
		else
		{
			return FRTSDKActiveModsInfo();
		}
	}
	return ActivateMods(gamemod, factionmods, mapmod, mutators);
}

TArray<URTSDKGameModDefinition*> URTSDKModManager::GetAllValidGameMods()
{
	TArray<URTSDKGameModDefinition*> retval;
	for (auto It = GameModsByName.CreateConstIterator(); It; ++It)
	{
		if (It->Value->bIsValid)
		{
			retval.Add(It->Value);
		}
	}
	return retval;
}

TArray<FName> URTSDKModManager::GetAllValidGameModNames()
{
	TArray<FName> retval;
	TArray<URTSDKGameModDefinition*> mods = GetAllValidGameMods();
	for (int32 i = 0; i < mods.Num(); i++)
	{
		retval.Add(mods[i]->ModDevName);
	}

	return retval;
}

TArray<URTSDKFactionModDefinition*> URTSDKModManager::GetAllValidFactionMods()
{
	TArray<URTSDKFactionModDefinition*> retval;
	for (auto It = FactionModsByName.CreateConstIterator(); It; ++It)
	{
		if (It->Value->bIsValid)
		{
			retval.Add(It->Value);
		}
	}
	return retval;
}

TArray<FName> URTSDKModManager::GetAllValidFactionModNames()
{
	TArray<FName> retval;
	TArray<URTSDKFactionModDefinition*> mods = GetAllValidFactionMods();
	for (int32 i = 0; i < mods.Num(); i++)
	{
		retval.Add(mods[i]->ModDevName);
	}

	return retval;
}

TArray<URTSDKMapModDefinition*> URTSDKModManager::GetAllValidMapMods()
{
	TArray<URTSDKMapModDefinition*> retval;
	for (auto It = MapModsByName.CreateConstIterator(); It; ++It)
	{
		if (It->Value->bIsValid)
		{
			retval.Add(It->Value);
		}
	}
	return retval;
}

TArray<FName> URTSDKModManager::GetAllValidMapModNames()
{
	TArray<FName> retval;
	TArray<URTSDKMapModDefinition*> mods = GetAllValidMapMods();
	for (int32 i = 0; i < mods.Num(); i++)
	{
		retval.Add(mods[i]->ModDevName);
	}

	return retval;
}

TArray<URTSDKMutatorDefinition*> URTSDKModManager::GetAllValidMutators()
{
	TArray<URTSDKMutatorDefinition*> retval;
	for (auto It = MutatorsByName.CreateConstIterator(); It; ++It)
	{
		if (It->Value->bIsValid)
		{
			retval.Add(It->Value);
		}
	}
	return retval;
}

TArray<FName> URTSDKModManager::GetAllValidMutatorNames()
{
	TArray<FName> retval;
	TArray<URTSDKMutatorDefinition*> mutators = GetAllValidMutators();
	for (int32 i = 0; i < mutators.Num(); i++)
	{
		retval.Add(mutators[i]->ModDevName);
	}

	return retval;
}

TArray<URTSDKConfigurableHUDDefinition*> URTSDKModManager::GetAllValidHUDs()
{
	TArray<URTSDKConfigurableHUDDefinition*> retval;
	for (auto It = ConfigurableHUDByName.CreateConstIterator(); It; ++It)
	{
		if (It->Value->bIsValid)
		{
			retval.Add(It->Value);
		}
	}
	return retval;
}

TArray<URTSDKConfigurableInputMappingDefinition*> URTSDKModManager::GetAllValidInputMappings()
{
	TArray<URTSDKConfigurableInputMappingDefinition*> retval;
	for (auto It = ConfigurableMappingsByName.CreateConstIterator(); It; ++It)
	{
		if (It->Value->bIsValid)
		{
			retval.Add(It->Value);
		}
	}
	return retval;
}

void URTSDKModManager::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	PendingGameFeatureCount--;
	if (PendingGameFeatureCount <= 0)
	{
		OnModsFullyActivated();
	}
}

void URTSDKModManager::OnModsFullyActivated()
{
	OnActiveModsReady.Broadcast(ActiveMods);
}
