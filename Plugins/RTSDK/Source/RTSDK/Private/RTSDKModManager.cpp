// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKModManager.h"
#include "RTSDKModDefinitionBase.h"
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

	FRTSDKRegisteredModsMap& features = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Feature);
	FRTSDKRegisteredModsMap& games = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Game);
	FRTSDKRegisteredModsMap& factions = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Faction);
	FRTSDKRegisteredModsMap& maps = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Map);
	FRTSDKRegisteredModsMap& mutators = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Mutator);	

	for (auto It = games.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = factions.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = maps.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	for (auto It = mutators.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}

	/*for (auto It = ConfigurableHUDByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildModDependencies(this);
	}*/

	for (auto It = factions.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	for (auto It = maps.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	for (auto It = mutators.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}

	//for (auto It = ConfigurableHUDByName.CreateIterator(); It; ++It)
	//{
	//	It->Value->BuildMod(this);
	//}

	for (auto It = games.ModsByName.CreateIterator(); It; ++It)
	{
		It->Value->BuildMod(this);
	}
}

void URTSDKModManager::RegisterMod(FName inModTypeName, URTSDKModDefinitionBase* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	if (inModDef->DevName.IsNone())
	{
		return;
	}
	FRTSDKRegisteredModsMap& mods = RegisteredModsByType.FindOrAdd(inModTypeName);
	if (mods.ModsByName.Contains(inModDef->DevName))
	{
		return;
	}
	mods.ModsByName.Add(inModDef->DevName, inModDef);
}

void URTSDKModManager::RegisterFeatureMod(URTSDKFeatureModDefinition* inModDef)
{
	RegisterMod(RTSDKModTypeNames::Feature, inModDef);
}

void URTSDKModManager::RegisterGameMod(URTSDKGameModDefinition* inModDef)
{
	RegisterMod(RTSDKModTypeNames::Game, inModDef);
}

void URTSDKModManager::RegisterFactionMod(URTSDKFactionModDefinition* inModDef)
{
	RegisterMod(RTSDKModTypeNames::Faction, inModDef);
}

void URTSDKModManager::RegisterMapMod(URTSDKMapModDefinition* inModDef)
{
	RegisterMod(RTSDKModTypeNames::Map, inModDef);
}

void URTSDKModManager::RegisterMutator(URTSDKMutatorDefinition* inModDef)
{
	RegisterMod(RTSDKModTypeNames::Mutator, inModDef);
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
	return Cast<URTSDKFeatureModDefinition>(GetMod(RTSDKModTypeNames::Feature, inModDevName));
}

URTSDKModDefinitionBase* URTSDKModManager::GetMod(FName inModTypeName, FName inModDevName) const
{
	if (RegisteredModsByType.Contains(inModTypeName))
	{
		return RegisteredModsByType[inModTypeName].GetModByName(inModDevName);
	}
	return nullptr;
}

URTSDKGameModDefinition* URTSDKModManager::GetGameModByName(FName inModDevName) const
{
	return Cast<URTSDKGameModDefinition>(GetMod(RTSDKModTypeNames::Game, inModDevName));
}

URTSDKFactionModDefinition* URTSDKModManager::GetFactionModByName(FName inModDevName) const
{
	return Cast<URTSDKFactionModDefinition>(GetMod(RTSDKModTypeNames::Faction, inModDevName));
}

URTSDKMapModDefinition* URTSDKModManager::GetMapModByName(FName inModDevName) const
{
	return Cast<URTSDKMapModDefinition>(GetMod(RTSDKModTypeNames::Map, inModDevName));
}

URTSDKMutatorDefinition* URTSDKModManager::GetMutatorByName(FName inModDevName) const
{
	return Cast<URTSDKMutatorDefinition>(GetMod(RTSDKModTypeNames::Mutator, inModDevName));
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
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Faction);
	if (mods == nullptr)
	{
		return TArray<URTSDKFactionModDefinition*>();
	}
	TArray<URTSDKFactionModDefinition*> retval;
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKFactionModDefinition* def = Cast<URTSDKFactionModDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract()) && (def->GetAssociatedMod() == inGameMod))
			{
				retval.Add(def);
			}
		}
	}
	return retval;
}

TArray<URTSDKMapModDefinition*> URTSDKModManager::GetMapModsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Map);
	if (mods == nullptr)
	{
		return TArray<URTSDKMapModDefinition*>();
	}
	TArray<URTSDKMapModDefinition*> retval;
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKMapModDefinition* def = Cast<URTSDKMapModDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract()) && (def->GetAssociatedMod() == inGameMod))
			{
				retval.Add(def);
			}
		}
	}
	return retval;
}

TArray<URTSDKMutatorDefinition*> URTSDKModManager::GetMutatorsByGameMod(URTSDKGameModDefinition* inGameMod) const
{
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Mutator);
	if (mods == nullptr)
	{
		return TArray<URTSDKMutatorDefinition*>();
	}
	TArray<URTSDKMutatorDefinition*> retval;
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKMutatorDefinition* def = Cast<URTSDKMutatorDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract()) && (def->GetAssociatedMod() == inGameMod))
			{
				retval.Add(def);
			}
		}
	}
	return retval;
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

FRTSDKActiveModsInfo URTSDKModManager::ActivateMods(URTSDKGameModDefinition* inGameMod, TArray<URTSDKFactionModDefinition*> inFactionMods, URTSDKMapModDefinition* inMapMod, TArray<URTSDKMutatorDefinition*> inMutators)
{
	if (inGameMod == nullptr)
	{
		return FRTSDKActiveModsInfo();//bad game mod input
	}
	if (!inGameMod->bIsValid)
	{
		return FRTSDKActiveModsInfo();//bad game mod, invalid
	}
	if (inMapMod == nullptr)
	{
		return FRTSDKActiveModsInfo();//bad map mod input
	}
	if (!inMapMod->bIsValid)
	{
		return FRTSDKActiveModsInfo();//bad game mod, invalid
	}
	if (inFactionMods.Num() == 0)
	{
		return FRTSDKActiveModsInfo();//no faction mods, even a game with 1 faction needs a faction defined!
	}
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		if (inFactionMods[i] == nullptr)
		{
			return FRTSDKActiveModsInfo();//bad faction mod input
		}
		if (!inFactionMods[i]->bIsValid)
		{
			return FRTSDKActiveModsInfo();//bad faction mod, invalid
		}
	}

	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		if (inMutators[i] == nullptr)
		{
			return FRTSDKActiveModsInfo();//bad mutator mod input
		}
		if (!inMutators[i]->bIsValid)
		{
			return FRTSDKActiveModsInfo();//bad mutator mod, invalid
		}
	}

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
	PendingLoadingMods.Empty(2 + inFactionMods.Num() + inMutators.Num());
	PendingLoadingMods.Add(inGameMod);
	inGameMod->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
	inGameMod->LoadMod(this);
	PendingLoadingMods.Add(inMapMod);
	inMapMod->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
	inMapMod->LoadMod(this);
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		PendingLoadingMods.Add(inFactionMods[i]);
		inFactionMods[i]->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
		inFactionMods[i]->LoadMod(this);
	}
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		PendingLoadingMods.Add(inMutators[i]);
		inMutators[i]->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
		inMutators[i]->LoadMod(this);
	}
	
	ActiveMods = retval;
	return retval;
}

FRTSDKActiveModsInfo URTSDKModManager::ActivateModsByName(FName inGameMod, TArray<FName> inFactionMods, FName inMapMod, TArray<FName> inMutators)
{
	FRTSDKRegisteredModsMap& features = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Feature);
	FRTSDKRegisteredModsMap& games = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Game);
	FRTSDKRegisteredModsMap& factions = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Faction);
	FRTSDKRegisteredModsMap& maps = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Map);
	FRTSDKRegisteredModsMap& mutators = RegisteredModsByType.FindOrAdd(RTSDKModTypeNames::Mutator);
	URTSDKGameModDefinition* gamemod = nullptr;
	if (games.ModsByName.Contains(inGameMod))
	{
		gamemod = Cast<URTSDKGameModDefinition>(games.ModsByName[inGameMod]);
	}
	else
	{
		return FRTSDKActiveModsInfo();
	}
	TArray<URTSDKFactionModDefinition*> factionmods;
	factionmods.Empty(inFactionMods.Num());
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		if (factions.ModsByName.Contains(inFactionMods[i]))
		{
			factionmods.Add(Cast<URTSDKFactionModDefinition>(factions.ModsByName[inFactionMods[i]]));
		}
		else
		{
			return FRTSDKActiveModsInfo();
		}
	}
	URTSDKMapModDefinition* mapmod = nullptr;
	if (maps.ModsByName.Contains(inMapMod))
	{
		mapmod = Cast<URTSDKMapModDefinition>(maps.ModsByName[inMapMod]);
	}
	else
	{
		return FRTSDKActiveModsInfo();
	}

	TArray<URTSDKMutatorDefinition*> mutatormods;
	mutatormods.Empty(inMutators.Num());
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		if (mutators.ModsByName.Contains(inMutators[i]))
		{
			mutatormods.Add(Cast<URTSDKMutatorDefinition>(mutators.ModsByName[inMutators[i]]));
		}
		else
		{
			return FRTSDKActiveModsInfo();
		}
	}
	return ActivateMods(gamemod, factionmods, mapmod, mutatormods);
}

TArray<URTSDKGameModDefinition*> URTSDKModManager::GetAllValidGameMods()
{
	TArray<URTSDKGameModDefinition*> retval;
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Game); 
	if (mods == nullptr)
	{
		return TArray<URTSDKGameModDefinition*>();
	}
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKGameModDefinition* def = Cast<URTSDKGameModDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract())) 
			{
				retval.Add(def);
			}
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
		retval.Add(mods[i]->DevName);
	}

	return retval;
}

TArray<URTSDKFactionModDefinition*> URTSDKModManager::GetAllValidFactionMods()
{
	TArray<URTSDKFactionModDefinition*> retval;
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Faction);
	if (mods == nullptr)
	{
		return TArray<URTSDKFactionModDefinition*>();
	}
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKFactionModDefinition* def = Cast<URTSDKFactionModDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract()))
			{
				retval.Add(def);
			}
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
		retval.Add(mods[i]->DevName);
	}

	return retval;
}

TArray<URTSDKMapModDefinition*> URTSDKModManager::GetAllValidMapMods()
{
	TArray<URTSDKMapModDefinition*> retval;
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Map);
	if (mods == nullptr)
	{
		return TArray<URTSDKMapModDefinition*>();
	}
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKMapModDefinition* def = Cast<URTSDKMapModDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract()))
			{
				retval.Add(def);
			}
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
		retval.Add(mods[i]->DevName);
	}

	return retval;
}

TArray<URTSDKMutatorDefinition*> URTSDKModManager::GetAllValidMutators()
{
	TArray<URTSDKMutatorDefinition*> retval;
	const FRTSDKRegisteredModsMap* mods = RegisteredModsByType.Find(RTSDKModTypeNames::Mutator);
	if (mods == nullptr)
	{
		return TArray<URTSDKMutatorDefinition*>();
	}
	for (auto It = mods->ModsByName.CreateConstIterator(); It; ++It)
	{
		URTSDKMutatorDefinition* def = Cast<URTSDKMutatorDefinition>(It->Value);
		if (def != nullptr)
		{
			if ((def->IsModValid()) && (!def->IsModAbstract()))
			{
				retval.Add(def);
			}
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
		retval.Add(mutators[i]->DevName);
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

void URTSDKModManager::EnqueueModForActivation(URTSDKModDefinitionBase* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	ModActivationQueue.AddUnique(inModDef);
}

void URTSDKModManager::ProgressActivationQueue()
{
	if (ModActivationQueue.Num() > 0)
	{
		URTSDKModDefinitionBase* mod = ModActivationQueue[0];
		ModActivationQueue.RemoveAt(0);
		if (mod != nullptr)
		{
			UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();

			gamefeatures.LoadAndActivateGameFeaturePlugin(mod->GameFeatureURL, FGameFeaturePluginLoadComplete::CreateLambda(
				[this, mod](const UE::GameFeatures::FResult& Result)
				{
					mod->NotifyQueuedActivation();
					ProgressActivationQueue();			
				}
			));
		}
		else
		{
			ProgressActivationQueue();
		}
	}
}

void URTSDKModManager::EnqueueModForDeactivation(URTSDKModDefinitionBase* inModDef)
{
	if (inModDef == nullptr)
	{
		return;
	}
	ModDeactivationQueue.AddUnique(inModDef);
}

void URTSDKModManager::ProgressDeactivationQueue()
{
	if (ModDeactivationQueue.Num() > 0)
	{
		URTSDKModDefinitionBase* mod = ModDeactivationQueue[0];
		ModDeactivationQueue.RemoveAt(0);
		if (mod != nullptr)
		{
			UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();

			gamefeatures.DeactivateGameFeaturePlugin(mod->GameFeatureURL, FGameFeaturePluginDeactivateComplete::CreateLambda
			(
				[this, mod](const UE::GameFeatures::FResult& Result)
				{
					mod->NotifyQueuedDeactivation();
					ProgressDeactivationQueue();
				}
			));
		}
		else
		{
			ProgressDeactivationQueue();
		}
	}
	/*else
	{
		OnModsFullyActivated(); //todo: on mods fully deactivated multicast
	}*/
}

void URTSDKModManager::OnModIsFullyLoaded(URTSDKModDefinitionBase* Sender)
{
	PendingLoadingMods.Remove(Sender);
	Sender->OnModFullyLoaded.RemoveAll(this);
	if (PendingLoadingMods.Num() <= 0)
	{
		BeginModActivation();
	}
}

void URTSDKModManager::OnModIsFullyActivated(URTSDKModDefinitionBase* Sender)
{
	PendingActivatingMods.Remove(Sender);
	Sender->OnModFullyActivated.RemoveAll(this);
	if (PendingActivatingMods.Num() <= 0)
	{
		OnModsFullyActivated();
	}
}

void URTSDKModManager::BeginModActivation()
{
	PendingActivatingMods.Empty(2 + ActiveMods.ActiveFactionMods.Num() + ActiveMods.ActiveMutators.Num());
	PendingActivatingMods.Add(ActiveMods.ActiveGameMod);
	ActiveMods.ActiveGameMod->OnModFullyActivated.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
	ActiveMods.ActiveGameMod->ActivateMod(this);
	for (int32 i = 0; i < ActiveMods.ActiveFactionMods.Num(); i++)
	{
		PendingActivatingMods.Add(ActiveMods.ActiveFactionMods[i]);
		ActiveMods.ActiveFactionMods[i]->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
		ActiveMods.ActiveFactionMods[i]->ActivateMod(this);
	}
	PendingActivatingMods.Add(ActiveMods.ActiveMapMod);
	ActiveMods.ActiveMapMod->OnModFullyActivated.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
	ActiveMods.ActiveMapMod->ActivateMod(this);
	for (int32 i = 0; i < ActiveMods.ActiveMutators.Num(); i++)
	{
		PendingActivatingMods.Add(ActiveMods.ActiveMutators[i]);
		ActiveMods.ActiveMutators[i]->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
		ActiveMods.ActiveMutators[i]->ActivateMod(this);
	}
	ProgressActivationQueue();
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

URTSDKModDefinitionBase* FRTSDKRegisteredModsMap::GetModByName(FName inModDevName) const
{
	if (ModsByName.Contains(inModDevName))
	{
		return ModsByName[inModDevName];
	}
	return nullptr;
}
