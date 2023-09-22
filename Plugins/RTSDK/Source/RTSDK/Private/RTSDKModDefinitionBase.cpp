// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKModDefinitionBase.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKModManager.h"
#include "GameFeaturesSubsystem.h"

void URTSDKModDefinitionBase::Init(const URTSDKGameFeatureData* inData)
{
	ModState = ERTSDKModStates::Building;
	if (inData != nullptr)
	{
		ParentModInfo.ModDevName = inData->ParentModDevName;
		DevName = inData->DevName;
		DisplayName = inData->DisplayName;
		bIsAbstractMod = inData->bIsAbstractMod;
		ModDependencyInfos.Empty();
		for (int32 i = 0; i < inData->FeatureDependencies.Num(); i++)
		{
			if (inData->FeatureDependencies[i].IsNone())
			{
				continue;
			}
			FRTSDKModDependencyInfo dependency;
			dependency.ModType = FName(TEXT("Feature"));
			dependency.ModDevName = inData->FeatureDependencies[i];
			ModDependencyInfos.Add(dependency);
		}
		GameFeatureName = inData->GetName();
		UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
		FString featureurl;
		if (gamefeatures.GetPluginURLByName(GameFeatureName, featureurl))
		{
			GameFeatureURL = featureurl;
			return;
		}
		else
		{
			//bad game feature name
			ModState = ERTSDKModStates::Invalid;
			return;
		}
	}
	else
	{
		//bad game feature data
		ModState = ERTSDKModStates::Invalid;
		return;
	}
}

void URTSDKModDefinitionBase::BuildModDependencies(URTSDKModManager* inModManager)
{
	if (ModState == ERTSDKModStates::Invalid)
	{
		return;
	}
	
	if ((!ParentModInfo.ModDevName.IsNone()) && (ParentModInfo.ModType.IsNone()))
	{
		URTSDKModDefinitionBase* parent = inModManager->GetMod(ParentModInfo.ModType, ParentModInfo.ModDevName);
		if (parent != nullptr)
		{
			ParentModInfo.ModDefinition = parent;
			ParentModInfo.ModDefinition->OnModFullyLoaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyLoaded);
			ParentModInfo.ModDefinition->OnModFullyUnloaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyUnloaded);
			ParentModInfo.ModDefinition->OnModFullyActivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyActivated);
			ParentModInfo.ModDefinition->OnModFullyDeactivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyDeactivated);
		}
		else
		{
			//missing parent
			ModState = ERTSDKModStates::Invalid;
			return;
		}
	}

	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		URTSDKModDefinitionBase* dependency = inModManager->GetMod(ModDependencyInfos[i].ModType, ModDependencyInfos[i].ModDevName);
		if (dependency != nullptr)
		{
			ModDependencyInfos[i].ModDefinition = dependency;
			ModDependencyInfos[i].ModDefinition->OnModFullyLoaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyLoaded);
			ModDependencyInfos[i].ModDefinition->OnModFullyUnloaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyUnloaded);
			ModDependencyInfos[i].ModDefinition->OnModFullyActivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyActivated);
			ModDependencyInfos[i].ModDefinition->OnModFullyDeactivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyDeactivated);
		}
		else
		{
			//missing mod dependency
			ModState = ERTSDKModStates::Invalid;
			return;
		}
	}
}

void URTSDKModDefinitionBase::BuildMod(URTSDKModManager* inModManager)
{
	if (ModState == ERTSDKModStates::Invalid)
	{
		return;
	}
		
	TArray<URTSDKModDefinitionBase*> allparents;
	URTSDKModDefinitionBase* currentouter = ParentModInfo.ModDefinition.Get();
	while (currentouter != nullptr)
	{
		if (currentouter->ModState == ERTSDKModStates::Invalid)
		{
			//bad parent
			ModState = ERTSDKModStates::Invalid;
			return;
		}
		if (allparents.Contains(currentouter))
		{
			//circular dependency
			ModState = ERTSDKModStates::Invalid;
			return;
		}
		allparents.Add(currentouter);
		currentouter = currentouter->ParentModInfo.ModDefinition.Get();
	}

	ModState = ERTSDKModStates::Registered;
}

void URTSDKModDefinitionBase::LoadMod(UObject* inCallerObject)
{
	//fail out if this mod and any parents and dependencies aren't valid
	if (!IsModValid())
	{
		return;
	}
	//fail out if the caller has already called for a load without calling for an unload (which means we're loaded or loading!)
	if (LoadDependingObjects.Contains(inCallerObject))
	{
		return;
	}
	//fail out if we are already loaded, obviously.
	if (ModState >= ERTSDKModStates::Loaded)
	{
		return;
	}
	ModState = ERTSDKModStates::Loading;
	LoadDependingObjects.Add(inCallerObject);
	if (ParentModInfo.ModDefinition != nullptr)
	{
		ParentModInfo.ModDefinition->LoadMod(this);
	}
	
	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		if (ModDependencyInfos[i].ModDefinition != nullptr)
		{
			ModDependencyInfos[i].ModDefinition->LoadMod(this);
		}
	}

	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();

	gamefeatures.LoadGameFeaturePlugin(GameFeatureURL, FGameFeaturePluginLoadComplete::CreateLambda(
		[this](const UE::GameFeatures::FResult& Result)
		{
			ModState = ERTSDKModStates::Loaded;
			if (IsModLoaded())
			{
				OnModFullyLoaded.Broadcast(this);
			}
		}
	));

	return;
}

void URTSDKModDefinitionBase::UnloadMod(UObject* inCallerObject)
{
	LoadDependingObjects.Remove(inCallerObject);
	
	if (LoadDependingObjects.Num() <= 0)
	{
		ModState = ERTSDKModStates::Unloading;
		if (ParentModInfo.ModDefinition != nullptr)
		{
			ParentModInfo.ModDefinition->UnloadMod(this);
		}

		for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
		{
			if (ModDependencyInfos[i].ModDefinition != nullptr)
			{
				ModDependencyInfos[i].ModDefinition->UnloadMod(this);
			}
		}

		UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();

		gamefeatures.UnloadGameFeaturePlugin(GameFeatureURL, FGameFeaturePluginUnloadComplete::CreateLambda(
			[this](const UE::GameFeatures::FResult& Result)
			{
				ModState = ERTSDKModStates::Registered;
				if (IsModUnloaded())
				{
					OnModFullyUnloaded.Broadcast(this);
				}
			}
		), true);
	}	
}

void URTSDKModDefinitionBase::ActivateMod(UObject* inCallerObject)
{
	//fail out if this mod and any parents and dependencies aren't loaded yet
	if (!IsModLoaded())
	{
		return;
	}

	if (ActivateDependingObjects.Contains(inCallerObject))
	{
		return;
	}

	ActivateDependingObjects.Add(inCallerObject);

	if (ModState >= ERTSDKModStates::Activating)
	{
		return;
	}
	ModState = ERTSDKModStates::Activating;

	if (ParentModInfo.ModDefinition != nullptr)
	{
		ParentModInfo.ModDefinition->ActivateMod(this);
	}

	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		if (ModDependencyInfos[i].ModDefinition != nullptr)
		{
			ModDependencyInfos[i].ModDefinition->ActivateMod(this);
		}
	}

	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	modmanager->EnqueueModForActivation(this);	
}

void URTSDKModDefinitionBase::DeactivateMod(UObject* inCallerObject)
{
	ActivateDependingObjects.Remove(inCallerObject);

	if (ModState <= ERTSDKModStates::Deactivating)
	{
		return;
	}

	if (ActivateDependingObjects.Num() <= 0)
	{
		ModState = ERTSDKModStates::Deactivating;

		if (ParentModInfo.ModDefinition != nullptr)
		{
			ParentModInfo.ModDefinition->DeactivateMod(this);
		}

		for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
		{
			if (ModDependencyInfos[i].ModDefinition != nullptr)
			{
				ModDependencyInfos[i].ModDefinition->DeactivateMod(this);
			}
		}

		URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
		modmanager->EnqueueModForDeactivation(this);
	}
}

bool URTSDKModDefinitionBase::IsModValid()
{
	//if we arent valid then that's that.
	if (ModState == ERTSDKModStates::Invalid)
	{
		return false;
	}

	//if parent is specified
	if ((!ParentModInfo.ModDevName.IsNone()) && (!ParentModInfo.ModType.IsNone()))
	{
		//if the mod has not been built yet/specified parent didn't exist we aren't valid
		if (ParentModInfo.ModDefinition == nullptr)
		{
			return false;
		}
		//if the parent is not valid we aren't valid either.
		if (!ParentModInfo.ModDefinition->IsModValid())
		{
			return false;
		}
	}
	//for each dependency
	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		//if the mod has not been built yet/specified dependency didn't exist we aren't valid
		if (ModDependencyInfos[i].ModDefinition == nullptr)
		{
			return false;
		}
		//if the dependency is not valid we aren't valid either.
		if (!ModDependencyInfos[i].ModDefinition->IsModValid())
		{
			return false;
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModLoaded()
{
	//if we arent loaded then that's that.
	if (ModState < ERTSDKModStates::Loaded)
	{
		return false;
	}

	//if parent is specified
	if ((!ParentModInfo.ModDevName.IsNone()) && (!ParentModInfo.ModType.IsNone()))
	{
		//if specified parent exists don't bother checking
		if (ParentModInfo.ModDefinition != nullptr)
		{
			//if the parent is not loaded we aren't loaded either.
			if (!ParentModInfo.ModDefinition->IsModLoaded())
			{
				return false;
			}
		}
	}
	//for each dependency
	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		//if specified dependency didn't exist don't bother
		if (ModDependencyInfos[i].ModDefinition == nullptr)
		{
			continue;
		}
		//if the dependency is not loaded we aren't loaded either.
		if (!ModDependencyInfos[i].ModDefinition->IsModLoaded())
		{
			return false;
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModUnloaded()
{
	//if we arent registered or below then that's that.
	if (ModState > ERTSDKModStates::Registered)
	{
		return false;
	}

	//if parent is specified
	if ((!ParentModInfo.ModDevName.IsNone()) && (!ParentModInfo.ModType.IsNone()))
	{
		//if specified parent exists don't bother checking
		if (ParentModInfo.ModDefinition != nullptr)
		{
			//if the parent is not loaded we aren't loaded either.
			if (!ParentModInfo.ModDefinition->IsModUnloaded())
			{
				return false;
			}
		}
	}
	//for each dependency
	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		//if specified dependency didn't exist don't bother
		if (ModDependencyInfos[i].ModDefinition == nullptr)
		{
			continue;
		}
		//if the dependency is not loaded we aren't loaded either.
		if (!ModDependencyInfos[i].ModDefinition->IsModUnloaded())
		{
			return false;
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModActivated()
{
	//if we arent activated then that's that.
	if (ModState < ERTSDKModStates::Activated)
	{
		return false;
	}

	//if parent is specified
	if ((!ParentModInfo.ModDevName.IsNone()) && (!ParentModInfo.ModType.IsNone()))
	{
		//if specified parent doesn't exist don't bother checking
		if (ParentModInfo.ModDefinition != nullptr)
		{
			//if the parent is not activated we aren't activated either.
			if (!ParentModInfo.ModDefinition->IsModActivated())
			{
				return false;
			}
		}
	}
	//for each dependency
	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		//if specified dependency didn't exist don't bother
		if (ModDependencyInfos[i].ModDefinition == nullptr)
		{
			continue;
		}
		//if the dependency is not activated we aren't activated either.
		if (!ModDependencyInfos[i].ModDefinition->IsModActivated())
		{
			return false;
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModDeactivated()
{
	//if we arent loaded or below then that's that.
	if (ModState > ERTSDKModStates::Loaded)
	{
		return false;
	}

	//if parent is specified
	if ((!ParentModInfo.ModDevName.IsNone()) && (!ParentModInfo.ModType.IsNone()))
	{
		//if specified parent exists don't bother checking
		if (ParentModInfo.ModDefinition != nullptr)
		{
			//if the parent is not loaded we aren't loaded either.
			if (!ParentModInfo.ModDefinition->IsModDeactivated())
			{
				return false;
			}
		}
	}
	//for each dependency
	for (int32 i = 0; i < ModDependencyInfos.Num(); i++)
	{
		//if specified dependency didn't exist don't bother
		if (ModDependencyInfos[i].ModDefinition == nullptr)
		{
			continue;
		}
		//if the dependency is not loaded we aren't loaded either.
		if (!ModDependencyInfos[i].ModDefinition->IsModDeactivated())
		{
			return false;
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModAbstract()
{
	return bIsAbstractMod;
}

void URTSDKModDefinitionBase::DependencyLoaded(URTSDKModDefinitionBase* Sender)
{
	if (ModState != ERTSDKModStates::Loading)
	{
		return;
	}
	if (IsModLoaded())
	{
		OnModFullyLoaded.Broadcast(this);
	}
}

void URTSDKModDefinitionBase::DependencyUnloaded(URTSDKModDefinitionBase* Sender)
{
	if (ModState != ERTSDKModStates::Unloading)
	{
		return;
	}
	if (IsModUnloaded())
	{
		OnModFullyUnloaded.Broadcast(this);
	}
}

void URTSDKModDefinitionBase::DependencyActivated(URTSDKModDefinitionBase* Sender)
{
	if (ModState != ERTSDKModStates::Activating)
	{
		return;
	}
	if (IsModActivated())
	{
		OnModFullyActivated.Broadcast(this);
	}
}

void URTSDKModDefinitionBase::DependencyDeactivated(URTSDKModDefinitionBase* Sender)
{
	if (ModState != ERTSDKModStates::Deactivating)
	{
		return;
	}
	if (IsModDeactivated())
	{
		OnModFullyDeactivated.Broadcast(this);
	}
}

void URTSDKModDefinitionBase::NotifyQueuedActivation()
{
	if (ModState != ERTSDKModStates::Activating)
	{
		return;
	}
	ModState = ERTSDKModStates::Activated;
	if (IsModActivated())
	{
		OnModFullyActivated.Broadcast(this);
	}
}

void URTSDKModDefinitionBase::NotifyQueuedDeactivation()
{
	if (ModState != ERTSDKModStates::Deactivating)
	{
		return;
	}
	ModState = ERTSDKModStates::Loaded;
	if (IsModDeactivated())
	{
		OnModFullyDeactivated.Broadcast(this);
	}
}
