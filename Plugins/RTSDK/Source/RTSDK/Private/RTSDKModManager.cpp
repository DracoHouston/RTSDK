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
#include "RTSDKMassModuleSettings.h"

void URTSDKModManager::BuildMods()
{
	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
	gamefeatures.ForEachRegisteredGameFeature<URTSDKModDefinitionBase>(
		[this](const URTSDKModDefinitionBase* inData)
		{
			URTSDKModDefinitionBase* mutablemod = const_cast<URTSDKModDefinitionBase*>(inData);
			FName modstype = mutablemod->GetModType();
			if ((modstype == RTSDKModTypeNames::Feature) ||
				(modstype == RTSDKModTypeNames::Game) ||
				(modstype == RTSDKModTypeNames::Map) ||
				(modstype == RTSDKModTypeNames::Faction) ||
				(modstype == RTSDKModTypeNames::Mutator))
			{
				mutablemod->InitMod(this);
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
	if (inModDef->GameFeatureName.IsEmpty())
	{
		return;
	}
	FRTSDKRegisteredModsMap& mods = RegisteredModsByType.FindOrAdd(inModTypeName);
	if (mods.ModsByName.Contains(inModDef->GameFeatureName))
	{
		return;
	}
	mods.ModsByName.Add(inModDef->GameFeatureName, inModDef);
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

URTSDKFeatureModDefinition* URTSDKModManager::GetFeatureModByName(FString inModDevName) const
{
	return Cast<URTSDKFeatureModDefinition>(GetMod(RTSDKModTypeNames::Feature, inModDevName));
}

URTSDKModDefinitionBase* URTSDKModManager::GetMod(FName inModTypeName, FString inModDevName) const
{
	if (RegisteredModsByType.Contains(inModTypeName))
	{
		return RegisteredModsByType[inModTypeName].GetModByName(inModDevName);
	}
	return nullptr;
}

URTSDKGameModDefinition* URTSDKModManager::GetGameModByName(FString inModDevName) const
{
	return Cast<URTSDKGameModDefinition>(GetMod(RTSDKModTypeNames::Game, inModDevName));
}

URTSDKFactionModDefinition* URTSDKModManager::GetFactionModByName(FString inModDevName) const
{
	return Cast<URTSDKFactionModDefinition>(GetMod(RTSDKModTypeNames::Faction, inModDevName));
}

URTSDKMapModDefinition* URTSDKModManager::GetMapModByName(FString inModDevName) const
{
	return Cast<URTSDKMapModDefinition>(GetMod(RTSDKModTypeNames::Map, inModDevName));
}

URTSDKMutatorDefinition* URTSDKModManager::GetMutatorByName(FString inModDevName) const
{
	return Cast<URTSDKMutatorDefinition>(GetMod(RTSDKModTypeNames::Mutator, inModDevName));
}
//
//URTSDKConfigurableHUDDefinition* URTSDKModManager::GetConfigurableHUDByName(FName inHUDDevName) const
//{
//	if (ConfigurableHUDByName.Contains(inHUDDevName))
//	{
//		return ConfigurableHUDByName[inHUDDevName];
//	}
//	return nullptr;
//}
//
//URTSDKConfigurableInputMappingDefinition* URTSDKModManager::GetConfigurableInputMappingByName(FName inMappingDevName) const
//{
//	if (ConfigurableMappingsByName.Contains(inMappingDevName))
//	{
//		return ConfigurableMappingsByName[inMappingDevName];
//	}
//	return nullptr;
//}

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

//TArray<URTSDKConfigurableHUDDefinition*> URTSDKModManager::GetConfigurableHUDsByGameMod(URTSDKGameModDefinition* inGameMod) const
//{
//	TArray<URTSDKConfigurableHUDDefinition*> huds;
//	for (auto It = ConfigurableHUDByName.CreateConstIterator(); It; ++It)
//	{
//		if ((It->Value->bIsValid) && (!It->Value->bIsAbstractHUD) && (It->Value->AssociatedGameMod == inGameMod))
//		{
//			huds.Add(It->Value);
//		}
//	}
//	return huds;
//}
//
//TArray<URTSDKConfigurableInputMappingDefinition*> URTSDKModManager::GetConfigurableMappingsByGameMod(URTSDKGameModDefinition* inGameMod) const
//{
//	TArray<URTSDKConfigurableInputMappingDefinition*> mappings;
//	for (auto It = ConfigurableMappingsByName.CreateConstIterator(); It; ++It)
//	{
//		if ((It->Value->bIsValid) && (It->Value->AssociatedGameMod == inGameMod))
//		{
//			mappings.Add(It->Value);
//		}
//	}
//	return mappings;
//}

FRTSDKActiveModsInfo URTSDKModManager::ActivateMods(UWorld* Caller, URTSDKGameModDefinition* inGameMod, TArray<URTSDKFactionModDefinition*> inFactionMods, URTSDKMapModDefinition* inMapMod, TArray<URTSDKMutatorDefinition*> inMutators)
{
	if (inGameMod == nullptr)
	{
		return FRTSDKActiveModsInfo();//bad game mod input
	}
	if (!inGameMod->IsModValid())
	{
		return FRTSDKActiveModsInfo();//bad game mod, invalid
	}
	if (inMapMod == nullptr)
	{
		return FRTSDKActiveModsInfo();//bad map mod input
	}
	if (!inMapMod->IsModValid())
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
		if (!inFactionMods[i]->IsModValid())
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
		if (!inMutators[i]->IsModValid())
		{
			return FRTSDKActiveModsInfo();//bad mutator mod, invalid
		}
	}

	FRTSDKActiveModsInfo& worldactivemods = ActiveMods.FindOrAdd(Caller);

	FRTSDKActiveModsInfo modstoactivate;

	modstoactivate.ActiveGameMod = inGameMod;
	modstoactivate.ActiveFactionMods = inFactionMods;
	modstoactivate.ActiveFactionMods.Sort
	(
		[](const URTSDKFactionModDefinition& A, const URTSDKFactionModDefinition& B)
		{
			return A.GameFeatureName.Compare(B.GameFeatureName) < 0;
		}
	);
	modstoactivate.ActiveMapMod = inMapMod;
	modstoactivate.ActiveMutators = inMutators;
	modstoactivate.ActiveMutators.Sort
	(
		[](const URTSDKMutatorDefinition& A, const URTSDKMutatorDefinition& B)
		{
			return A.GameFeatureName.Compare(B.GameFeatureName) < 0;
		}
	);
	modstoactivate.MapLevelAsset = inMapMod->AssociatedLevel;
	if (inMapMod->GameMenuWidgetClass != nullptr)
	{
		modstoactivate.GameMenuOverrideClass = inMapMod->GameMenuWidgetClass;
	}
	else if (inGameMod->GameMenuWidgetClass != nullptr)
	{
		modstoactivate.GameMenuOverrideClass = inGameMod->GameMenuWidgetClass;
	}


	const URTSDKMassModuleSettings* rtsdkmasssettings = GetMutableDefault<URTSDKMassModuleSettings>();

	modstoactivate.ActiveSimProcessingPhase = rtsdkmasssettings->SimProcessingPhaseConfig;
	//modstoactivate.SubscribedWorlds.AddUnique(Caller);
	//if we have another set of active mods
	if (worldactivemods.IsValid())
	{
		//if its the same mod set as those that are active, don't bother.
		if (worldactivemods == modstoactivate)
		{
			return worldactivemods;
		}
		//TODO: deactivate the old ones
	}

	worldactivemods = modstoactivate;
		
	FRTSDKModsArray& worldpendingloadmods = PendingLoadingMods.FindOrAdd(Caller);
	
	worldpendingloadmods.Mods.Add(inGameMod);
	inGameMod->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
	worldpendingloadmods.Mods.Add(inMapMod);
	inMapMod->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
	for (int32 i = 0; i < inFactionMods.Num(); i++)
	{
		worldpendingloadmods.Mods.Add(inFactionMods[i]);
		inFactionMods[i]->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
	}
	for (int32 i = 0; i < inMutators.Num(); i++)
	{
		worldpendingloadmods.Mods.Add(inMutators[i]);
		inMutators[i]->OnModFullyLoaded.AddDynamic(this, &URTSDKModManager::OnModIsFullyLoaded);
	}
	//we copy the top level mod load queue before calling load because
	//sometimes plugins will load so fast we don't get to finish all load calls before
	//they finish, which would then trigger activation of those mods, and make this
	//loop bug out on a race condition, eww!
	TArray<URTSDKModDefinitionBase*> modstoload = worldpendingloadmods.Mods;

	for (int32 i = 0; i < modstoload.Num(); i++)
	{
		modstoload[i]->LoadMod(Caller, this);
	}
	
	
	return worldactivemods;
}

FRTSDKActiveModsInfo URTSDKModManager::ActivateModsByName(UWorld* Caller, FString inGameMod, TArray<FString> inFactionMods, FString inMapMod, TArray<FString> inMutators)
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
	return ActivateMods(Caller, gamemod, factionmods, mapmod, mutatormods);
}

void URTSDKModManager::DeactivateMods(UWorld* Caller)
{
	FRTSDKActiveModsInfo& worldactivemods = ActiveMods.FindOrAdd(Caller);

	for (int32 i = worldactivemods.ActiveMutators.Num() - 1; i >= 0; i--)
	{
		if (worldactivemods.ActiveMutators[i] != nullptr)
		{
			worldactivemods.ActiveMutators[i]->DeactivateMod(Caller, this);
		}
	}

	if (worldactivemods.ActiveMapMod != nullptr)
	{
		worldactivemods.ActiveMapMod->DeactivateMod(Caller, this);
	}

	for (int32 i = worldactivemods.ActiveFactionMods.Num() - 1; i >= 0; i--)
	{
		if (worldactivemods.ActiveFactionMods[i] != nullptr)
		{
			worldactivemods.ActiveFactionMods[i]->DeactivateMod(Caller, this);
		}
	}

	if (worldactivemods.ActiveGameMod != nullptr)
	{
		worldactivemods.ActiveGameMod->DeactivateMod(Caller, this);
	}

	worldactivemods = FRTSDKActiveModsInfo();
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

TArray<FString> URTSDKModManager::GetAllValidGameModNames()
{
	TArray<FString> retval;
	TArray<URTSDKGameModDefinition*> mods = GetAllValidGameMods();
	for (int32 i = 0; i < mods.Num(); i++)
	{
		retval.Add(mods[i]->GameFeatureName);
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

TArray<FString> URTSDKModManager::GetAllValidFactionModNames()
{
	TArray<FString> retval;
	TArray<URTSDKFactionModDefinition*> mods = GetAllValidFactionMods();
	for (int32 i = 0; i < mods.Num(); i++)
	{
		retval.Add(mods[i]->GameFeatureName);
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

TArray<FString> URTSDKModManager::GetAllValidMapModNames()
{
	TArray<FString> retval;
	TArray<URTSDKMapModDefinition*> mods = GetAllValidMapMods();
	for (int32 i = 0; i < mods.Num(); i++)
	{
		retval.Add(mods[i]->GameFeatureName);
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

TArray<FString> URTSDKModManager::GetAllValidMutatorNames()
{
	TArray<FString> retval;
	TArray<URTSDKMutatorDefinition*> mutators = GetAllValidMutators();
	for (int32 i = 0; i < mutators.Num(); i++)
	{
		retval.Add(mutators[i]->GameFeatureName);
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
//
//void URTSDKModManager::EnqueueModForActivation(URTSDKModDefinitionBase* inModDef)
//{
//	if (inModDef == nullptr)
//	{
//		return;
//	}
//	ModActivationQueue.AddUnique(inModDef);
//}
//
//void URTSDKModManager::ProgressActivationQueue()
//{
//	if (ModActivationQueue.Num() > 0)
//	{
//		URTSDKModDefinitionBase* mod = ModActivationQueue[0];
//		ModActivationQueue.RemoveAt(0);
//		if (mod != nullptr)
//		{
//
//			UE_LOG(LogTemp, Log, TEXT("Activating: mod %s"), *mod->DevName.ToString());
//			UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
//
//			gamefeatures.LoadAndActivateGameFeaturePlugin(mod->GameFeatureURL, FGameFeaturePluginLoadComplete::CreateLambda(
//				[this, mod](const UE::GameFeatures::FResult& Result)
//				{
//					mod->NotifyQueuedActivation();
//					ProgressActivationQueue();			
//				}
//			));
//		}
//		else
//		{
//			ProgressActivationQueue();
//		}
//	}
//}
//
//void URTSDKModManager::EnqueueModForDeactivation(URTSDKModDefinitionBase* inModDef)
//{
//	if (inModDef == nullptr)
//	{
//		return;
//	}
//	ModDeactivationQueue.AddUnique(inModDef);
//}
//
//void URTSDKModManager::ProgressDeactivationQueue()
//{
//	if (ModDeactivationQueue.Num() > 0)
//	{
//		URTSDKModDefinitionBase* mod = ModDeactivationQueue[0];
//		ModDeactivationQueue.RemoveAt(0);
//		if (mod != nullptr)
//		{
//			UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
//
//			gamefeatures.DeactivateGameFeaturePlugin(mod->GameFeatureURL, FGameFeaturePluginDeactivateComplete::CreateLambda
//			(
//				[this, mod](const UE::GameFeatures::FResult& Result)
//				{
//					mod->NotifyQueuedDeactivation();
//					ProgressDeactivationQueue();
//				}
//			));
//		}
//		else
//		{
//			ProgressDeactivationQueue();
//		}
//	}
//	/*else
//	{
//		OnModsFullyActivated(); //todo: on mods fully deactivated multicast
//	}*/
//}

void URTSDKModManager::AddMassProcessorToActiveSimPhase(UWorld* WorldContext, URTSDKModDefinitionBase* Caller, UMassProcessor* inProcessorCDO)
{
	if (Caller == nullptr)
	{
		return;
	}
	if (inProcessorCDO == nullptr)
	{
		return;
	}
	if (!Caller->IsModValid())
	{
		return;
	}
	FRTSDKActiveModsInfo& worldmods = ActiveMods.FindOrAdd(WorldContext);
	worldmods.AdditionalProcessorCDOs.FindOrAdd(inProcessorCDO).Mods.AddUnique(Caller);
}

FMassProcessingPhaseConfig& URTSDKModManager::GetActiveSimPhaseConfig(UWorld* WorldContext)
{
	FRTSDKActiveModsInfo& worldmods = ActiveMods.FindOrAdd(WorldContext);
	return worldmods.ActiveSimProcessingPhase;
}

void URTSDKModManager::OnModIsFullyLoaded(UWorld* WorldContext, URTSDKModDefinitionBase* Sender)
{
	FRTSDKModsArray& mods = PendingLoadingMods.FindOrAdd(WorldContext);
	mods.Mods.Remove(Sender);
	Sender->OnModFullyLoaded.RemoveAll(this);
	if (mods.Mods.Num() <= 0)
	{
		BeginModActivation(WorldContext);
	}
}

void URTSDKModManager::OnModIsFullyActivated(UWorld* WorldContext, URTSDKModDefinitionBase* Sender)
{
	FRTSDKModsArray& mods = PendingActivatingMods.FindOrAdd(WorldContext);
	mods.Mods.Remove(Sender);
	Sender->OnModFullyActivated.RemoveAll(this);
	if (mods.Mods.Num() <= 0)
	{
		OnModsFullyActivated(WorldContext);
	}
}

void URTSDKModManager::BeginModActivation(UWorld* WorldContext)
{
	FRTSDKActiveModsInfo& worldmods = ActiveMods.FindOrAdd(WorldContext);
	//PendingActivatingMods.Empty(2 + worldmods.ActiveFactionMods.Num() + worldmods.ActiveMutators.Num());
	//PendingActivatingMods.Add(worldmods.ActiveGameMod);
	//worldmods.ActiveGameMod->OnModFullyActivated.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
	worldmods.ActiveGameMod->ActivateMod(WorldContext, this);
	for (int32 i = 0; i < worldmods.ActiveFactionMods.Num(); i++)
	{
		//PendingActivatingMods.Add(worldmods.ActiveFactionMods[i]);
		//worldmods.ActiveFactionMods[i]->OnModFullyActivated.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
		worldmods.ActiveFactionMods[i]->ActivateMod(WorldContext, this);
	}
	//PendingActivatingMods.Add(ActiveMods.ActiveMapMod);
	//worldmods.ActiveMapMod->OnModFullyActivated.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
	worldmods.ActiveMapMod->ActivateMod(WorldContext, this);
	for (int32 i = 0; i < worldmods.ActiveMutators.Num(); i++)
	{
		//PendingActivatingMods.Add(ActiveMods.ActiveMutators[i]);
		//worldmods.ActiveMutators[i]->OnModFullyActivated.AddDynamic(this, &URTSDKModManager::OnModIsFullyActivated);
		worldmods.ActiveMutators[i]->ActivateMod(WorldContext, this);
	}
	OnModsFullyActivated(WorldContext);
	//ProgressActivationQueue();
}

void URTSDKModManager::OnModsFullyActivated(UWorld* WorldContext)
{
	FRTSDKActiveModsInfo& worldmods = ActiveMods.FindOrAdd(WorldContext);
	for (auto It = worldmods.AdditionalProcessorCDOs.CreateConstIterator(); It; ++It)
	{
		if (It->Value.Mods.Num() > 0)
		{
			worldmods.ActiveSimProcessingPhase.ProcessorCDOs.AddUnique(It->Key);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Mods are fully activated!"));
	OnActiveModsReady.Broadcast(WorldContext, worldmods);
}

URTSDKModDefinitionBase* FRTSDKRegisteredModsMap::GetModByName(FString inModName) const
{
	if (ModsByName.Contains(inModName))
	{
		return ModsByName[inModName];
	}
	return nullptr;
}

bool FRTSDKActiveModsInfo::IsValid()
{
	return (ActiveGameMod != nullptr) &&
		(ActiveMapMod != nullptr) &&
		(!ActiveFactionMods.Contains(nullptr)) &&
		(!ActiveMutators.Contains(nullptr)) &&
		(MapLevelAsset.IsValid());
}

bool FRTSDKActiveModsInfo::operator==(const FRTSDKActiveModsInfo& Other)
{
	
	return (ActiveFactionMods == Other.ActiveFactionMods) && 
		(ActiveGameMod == Other.ActiveGameMod) && 
		(ActiveMapMod == Other.ActiveMapMod) && 
		(ActiveMutators == Other.ActiveMutators) &&
		(MapLevelAsset == Other.MapLevelAsset) &&
		(GameMenuOverrideClass == Other.GameMenuOverrideClass);
}
