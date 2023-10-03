// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKModDefinitionBase.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKModManager.h"
#include "RTSDKFeatureActionBase.h"
#include "GameFeaturesSubsystem.h"

void URTSDKModDefinitionBase::InitMod(URTSDKModManager* inModManager)
{
	ModState = ERTSDKModStates::Building;
	GameFeatureName = GetName();
	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
	FString featureurl;
	if (gamefeatures.GetPluginURLByName(GameFeatureName, featureurl))
	{
		GameFeatureURL = featureurl;
		//return;
	}
	else
	{
		//bad game feature name
		UE_LOG(LogTemp, Log, TEXT("Mod %s was not found by game features, please ensure the plugin is installed and registered before rebuilding."), *GameFeatureName);
		ModState = ERTSDKModStates::Invalid;
		return;
	}
	inModManager->RegisterMod(GetModType(), this);
}

void URTSDKModDefinitionBase::BuildModDependencies(URTSDKModManager* inModManager)
{
	if (ModState == ERTSDKModStates::Invalid)
	{
		return;
	}
	
	if (ParentMod.IsValid())
	{
		URTSDKModDefinitionBase* parent = ParentMod.Get();

		if (parent != nullptr)
		{
#if !WITH_EDITORONLY_DATA
			ParentModPtr = parent;
#endif
			parent->OnModFullyLoaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyLoaded);
			parent->OnModFullyUnloaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyUnloaded);
			//parent->OnModFullyActivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyActivated);
			//parent->OnModFullyDeactivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyDeactivated);
		}
		else
		{
			//missing parent
			UE_LOG(LogTemp, Log, TEXT("%s mod has a bad parent!"), *GameFeatureName);
			ModState = ERTSDKModStates::Invalid;
			return;
		}
	}

	TArray<URTSDKModDefinitionBase*> dependencies;
	dependencies.Empty(FeatureDependencies.Num());

	
	for (int32 i = 0; i < FeatureDependencies.Num(); i++)
	{
		URTSDKModDefinitionBase* dependency = FeatureDependencies[i].Get();
		if (dependency != nullptr)
		{
			if (!dependencies.Contains(dependency))
			{
				dependencies.AddUnique(dependency);
				dependency->OnModFullyLoaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyLoaded);
				dependency->OnModFullyUnloaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyUnloaded);
				//dependency->OnModFullyActivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyActivated);
				//dependency->OnModFullyDeactivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyDeactivated);
			}
		}
		else
		{
			//missing mod dependency
			UE_LOG(LogTemp, Log, TEXT("Mod %s has a bad dependency"), *GameFeatureName);
			ModState = ERTSDKModStates::Invalid;
			return;
		}
	}
#if !WITH_EDITORONLY_DATA
	FeatureDependencyPtrs = dependencies;
#endif
}

void URTSDKModDefinitionBase::BuildMod(URTSDKModManager* inModManager)
{
	if (ModState == ERTSDKModStates::Invalid)
	{
		return;
	}
		
	TArray<URTSDKModDefinitionBase*> allparents;
	URTSDKModDefinitionBase* currentouter = GetParentMod();
	while (currentouter != nullptr)
	{
		if (currentouter->ModState == ERTSDKModStates::Invalid)
		{
			//bad parent

			UE_LOG(LogTemp, Log, TEXT("Mod %s failed to build because parent mod %s failed to build"), *GameFeatureName, *currentouter->GameFeatureName);
			ModState = ERTSDKModStates::Invalid;
			return;
		}
		if (allparents.Contains(currentouter))
		{
			//circular dependency

			UE_LOG(LogTemp, Log, TEXT("Mod %s failed to build because of a circular dependency on parent mod %s"), *GameFeatureName, *currentouter->GameFeatureName);
			ModState = ERTSDKModStates::Invalid;
			return;
		}
		allparents.Add(currentouter);
		currentouter = currentouter->GetParentMod();
	}

	ModState = ERTSDKModStates::Registered;
}

void URTSDKModDefinitionBase::LoadMod(UWorld* inWorldContext, UObject* inCallerObject)
{
	if (!IsModValid())
	{
		return;
	}

	FRTSDKModDependentsArray& worldloaddepenents = LoadDependingObjects.FindOrAdd(inWorldContext);

	if (worldloaddepenents.Dependents.Contains(inCallerObject))
	{
		return;
	}

	UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
	if (ModState == ERTSDKModStates::Unloading)
	{
		gamefeatures.CancelGameFeatureStateChange(GameFeatureURL, FGameFeaturePluginChangeStateComplete::CreateLambda(
			[this, inWorldContext, inCallerObject](const UE::GameFeatures::FResult& Result)
			{
				FRTSDKModDependentsArray& dependents = LoadDependingObjects.FindOrAdd(inWorldContext);
				dependents.Dependents.Remove(inCallerObject);
				LoadMod(inWorldContext, inCallerObject);
			}
		));
	}

	bool worldfirst = worldloaddepenents.Dependents.Num() == 0;
	worldloaddepenents.Dependents.Add(inCallerObject);
	
	if (!worldfirst)
	{
		return;
	}	

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		parent->LoadMod(inWorldContext, this);
	}
	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		if (dependencies[i] != nullptr)
		{
			dependencies[i]->LoadMod(inWorldContext, this);
		}
	}

	if (ModState == ERTSDKModStates::Loading)
	{
		return;
	}

	if (ModState == ERTSDKModStates::Loaded)
	{
		//UE_LOG(LogTemp, Log, TEXT("Loaded: mod %s"), *GameFeatureName);
		OnModFullyLoaded.Broadcast(inWorldContext, this);
		return;
	}

	ModState = ERTSDKModStates::Loading;
	UE_LOG(LogTemp, Log, TEXT("Loading: mod %s"), *GameFeatureName);	

	gamefeatures.LoadGameFeaturePlugin(GameFeatureURL, FGameFeaturePluginLoadComplete::CreateLambda(
		[this, inWorldContext](const UE::GameFeatures::FResult& Result)
		{
			ModState = ERTSDKModStates::Loaded;
			if (IsModLoaded())
			{
				UE_LOG(LogTemp, Log, TEXT("Loaded: mod %s"), *GameFeatureName);
				OnModFullyLoaded.Broadcast(inWorldContext, this);
			}
		}
	));

	return;
}

void URTSDKModDefinitionBase::UnloadMod(UWorld* inWorldContext, UObject* inCallerObject)
{
	FRTSDKModDependentsArray& worldloaddepenents = LoadDependingObjects.FindOrAdd(inWorldContext);
	worldloaddepenents.Dependents.Remove(inCallerObject);

	if (worldloaddepenents.Dependents.Num() == 0)
	{
		URTSDKModDefinitionBase* parent = GetParentMod();
		if (parent != nullptr)
		{
			parent->UnloadMod(inWorldContext, this);
		}
		TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
		for (int32 i = 0; i < dependencies.Num(); i++)
		{
			if (dependencies[i] != nullptr)
			{
				dependencies[i]->UnloadMod(inWorldContext, this);
			}
		}
	}
	bool shouldgarbagecollect = true;
	for (auto It = LoadDependingObjects.CreateConstIterator(); It; ++It)
	{
		if (It->Value.Dependents.Num() > 0)
		{
			shouldgarbagecollect = false;
		}
	}

	if (shouldgarbagecollect)
	{
		ModState = ERTSDKModStates::Unloading;
		UE_LOG(LogTemp, Log, TEXT("Unloading: mod %s"), *GameFeatureName);
		UGameFeaturesSubsystem& gamefeatures = UGameFeaturesSubsystem::Get();
		gamefeatures.UnloadGameFeaturePlugin(GameFeatureURL, FGameFeaturePluginUnloadComplete::CreateLambda(
			[this, inWorldContext](const UE::GameFeatures::FResult& Result)
			{
				ModState = ERTSDKModStates::Registered;
				if (IsModUnloaded())
				{
					UE_LOG(LogTemp, Log, TEXT("Unloaded: mod %s"), *GameFeatureName);
					OnModFullyUnloaded.Broadcast(inWorldContext, this);
				}
			}
		), true);
	}	
}

void URTSDKModDefinitionBase::ActivateMod(UWorld* inWorldContext, UObject* inCallerObject)
{
	if (!IsModLoaded())
	{
		return;
	}

	FRTSDKModDependentsArray& worldsdependents = ActivateDependingObjects.FindOrAdd(inWorldContext);
	if (worldsdependents.Dependents.Contains(inCallerObject))
	{
		return;
	}
	worldsdependents.Dependents.Add(inCallerObject);

	ERTSDKWorldModStates& worldmodstate = ModWorldStates.FindOrAdd(inWorldContext);
	if (worldmodstate != ERTSDKWorldModStates::Inactive)
	{
		return;
	}

	worldmodstate = ERTSDKWorldModStates::Activating;
	UE_LOG(LogTemp, Log, TEXT("Activating mod: %s"), *GameFeatureName);

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		parent->ActivateMod(inWorldContext, this);
	}

	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		if (dependencies[i] != nullptr)
		{
			dependencies[i]->ActivateMod(inWorldContext, this);
		}
	}

	for (auto It = Actions.CreateIterator(); It; ++It)
	{
		URTSDKFeatureActionBase* action = Cast<URTSDKFeatureActionBase>(It->Get());
		if (action != nullptr)
		{
			action->OnGameFeatureActivating(inWorldContext);
		}
	}

	worldmodstate = ERTSDKWorldModStates::Activated;
	UE_LOG(LogTemp, Log, TEXT("Activated mod: %s"), *GameFeatureName);
	OnModFullyActivated.Broadcast(inWorldContext, this);
}

void URTSDKModDefinitionBase::DeactivateMod(UWorld* inWorldContext, UObject* inCallerObject)
{
	ERTSDKWorldModStates& worldmodstate = ModWorldStates.FindOrAdd(inWorldContext);
	if (worldmodstate != ERTSDKWorldModStates::Activated)
	{
		return;
	}
	FRTSDKModDependentsArray& worldsdependents = ActivateDependingObjects.FindOrAdd(inWorldContext);
	worldsdependents.Dependents.Remove(inCallerObject);
	
	if (worldsdependents.Dependents.Num() <= 0)
	{
		worldmodstate = ERTSDKWorldModStates::Deactivating;
		UE_LOG(LogTemp, Log, TEXT("Deactivating mod: %s"), *GameFeatureName);

		URTSDKModDefinitionBase* parent = GetParentMod();
		if (parent != nullptr)
		{
			parent->DeactivateMod(inWorldContext, this);
		}

		TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
		for (int32 i = 0; i < dependencies.Num(); i++)
		{
			if (dependencies[i] != nullptr)
			{
				dependencies[i]->DeactivateMod(inWorldContext, this);
			}
		}
		
		for (auto It = Actions.CreateIterator(); It; ++It)
		{
			URTSDKFeatureActionBase* action = Cast<URTSDKFeatureActionBase>(It->Get());
			if (action != nullptr)
			{
				action->OnGameFeatureDeactivating(inWorldContext);
			}
		}

		worldmodstate = ERTSDKWorldModStates::Inactive;
		UE_LOG(LogTemp, Log, TEXT("Deactivated mod: %s"), *GameFeatureName);
		OnModFullyDeactivated.Broadcast(inWorldContext, this);
	}
}

bool URTSDKModDefinitionBase::IsModValid()
{
	if (ModState == ERTSDKModStates::Invalid)
	{
		return false;
	}

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		if (!parent->IsModValid())
		{
			return false;
		}
	}
	
	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		if (dependencies[i] != nullptr)
		{
			if (!dependencies[i]->IsModValid())
			{
				return false;
			}
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModLoaded()
{
	if (ModState < ERTSDKModStates::Loaded)
	{
		return false;
	}

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		if (!parent->IsModLoaded())
		{
			return false;
		}
	}
	
	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		if (dependencies[i] != nullptr)
		{
			if (!dependencies[i]->IsModLoaded())
			{
				return false;
			}
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModUnloaded()
{
	if (ModState > ERTSDKModStates::Registered)
	{
		return false;
	}

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		if (!parent->IsModUnloaded())
		{
			return false;
		}
	}

	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		if (dependencies[i] != nullptr)
		{
			if (!dependencies[i]->IsModUnloaded())
			{
				return false;
			}
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModActivated(UWorld* WorldContext)
{
	ERTSDKWorldModStates& worldmodstate = ModWorldStates.FindOrAdd(WorldContext);
	
	if (worldmodstate < ERTSDKWorldModStates::Activated)
	{
		return false;
	}

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		if (!parent->IsModActivated(WorldContext))
		{
			return false;
		}
	}

	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		//if the mod has not been built yet/specified dependency didn't exist we aren't valid
		if (dependencies[i] != nullptr)
		{
			if (!dependencies[i]->IsModActivated(WorldContext))
			{
				return false;
			}
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModDeactivated(UWorld* WorldContext)
{
	ERTSDKWorldModStates& worldmodstate = ModWorldStates.FindOrAdd(WorldContext);

	if (worldmodstate > ERTSDKWorldModStates::Inactive)
	{
		return false;
	}

	URTSDKModDefinitionBase* parent = GetParentMod();
	if (parent != nullptr)
	{
		if (!parent->IsModDeactivated(WorldContext))
		{
			return false;
		}
	}

	TArray<URTSDKModDefinitionBase*> dependencies = GetModDependencies();
	for (int32 i = 0; i < dependencies.Num(); i++)
	{
		//if the mod has not been built yet/specified dependency didn't exist we aren't valid
		if (dependencies[i] != nullptr)
		{
			if (!dependencies[i]->IsModDeactivated(WorldContext))
			{
				return false;
			}
		}
	}

	return true;
}

bool URTSDKModDefinitionBase::IsModAbstract()
{
	return bIsAbstractMod;
}

URTSDKModDefinitionBase* URTSDKModDefinitionBase::GetParentMod()
{
#if !WITH_EDITORONLY_DATA
	return ParentModPtr;
#else
	return ParentMod.Get();
#endif
	
}

TArray<URTSDKModDefinitionBase*> URTSDKModDefinitionBase::GetModDependencies()
{
#if !WITH_EDITORONLY_DATA
	return FeatureDependencyPtrs;
#else
	TArray<URTSDKModDefinitionBase*> retval;
	retval.Empty(FeatureDependencies.Num());
	for (int32 i = 0; i < FeatureDependencies.Num(); i++)
	{
		retval.Add(FeatureDependencies[i].Get());
	}
	return retval;
#endif
	
}

FName URTSDKModDefinitionBase::GetModType() const
{
	return FName();
}

void URTSDKModDefinitionBase::DependencyLoaded(UWorld* WorldContext, URTSDKModDefinitionBase* Sender)
{
	if (ModState != ERTSDKModStates::Loading)
	{
		return;
	}
	if (IsModLoaded())
	{
		OnModFullyLoaded.Broadcast(WorldContext, this);
	}
}

void URTSDKModDefinitionBase::DependencyUnloaded(UWorld* WorldContext, URTSDKModDefinitionBase* Sender)
{
	if (ModState != ERTSDKModStates::Unloading)
	{
		return;
	}
	if (IsModUnloaded())
	{
		OnModFullyUnloaded.Broadcast(WorldContext, this);
	}
}
//
//void URTSDKModDefinitionBase::DependencyActivated(UWorld* WorldContext, URTSDKModDefinitionBase* Sender)
//{
//	ERTSDKWorldModStates& worldmodstate = ModWorldStates.FindOrAdd(WorldContext);
//	if (worldmodstate != ERTSDKWorldModStates::Activating)
//	{
//		return;
//	}
//	if (IsModActivated(WorldContext))
//	{
//		OnModFullyActivated.Broadcast(WorldContext, this);
//	}
//}
//
//void URTSDKModDefinitionBase::DependencyDeactivated(UWorld* WorldContext, URTSDKModDefinitionBase* Sender)
//{
//	ERTSDKWorldModStates& worldmodstate = ModWorldStates.FindOrAdd(WorldContext);
//	if (worldmodstate != ERTSDKWorldModStates::Deactivating)
//	{
//		return;
//	}
//	if (IsModDeactivated())
//	{
//		OnModFullyDeactivated.Broadcast(WorldContext, this);
//	}
//}

//void URTSDKModDefinitionBase::Internal_ActivateInWorld(UWorld* WorldContext)
//{
//}

void URTSDKAssociatedModDefinitionBase::BuildModDependencies(URTSDKModManager* inModManager)
{
	Super::BuildModDependencies(inModManager);
	if (ModState == ERTSDKModStates::Invalid)
	{
		return;
	}

	if (AssociatedMod.IsValid())
	{
		URTSDKModDefinitionBase* associatedmod = AssociatedMod.Get();

		if (associatedmod != nullptr)
		{
			if (associatedmod->GetModType() == GetAllowedAssociatedModType())
			{
#if !WITH_EDITORONLY_DATA
				AssociatedModPtr = associatedmod;
#endif
				associatedmod->OnModFullyLoaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyLoaded);
				associatedmod->OnModFullyUnloaded.AddDynamic(this, &URTSDKModDefinitionBase::DependencyUnloaded);
				//associatedmod->OnModFullyActivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyActivated);
				//associatedmod->OnModFullyDeactivated.AddDynamic(this, &URTSDKModDefinitionBase::DependencyDeactivated);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s mod has an association with %s mod which is not a %s type mod!"), 
					*GameFeatureName, 
					*associatedmod->GameFeatureName, 
					*GetAllowedAssociatedModType().ToString());
				ModState = ERTSDKModStates::Invalid;
				return;
			}
		}
		else
		{
			//missing associated mod
			UE_LOG(LogTemp, Log, TEXT("%s mod has a bad associated mod!"), *GameFeatureName);
			ModState = ERTSDKModStates::Invalid;
			return;
		}
	}
}

URTSDKModDefinitionBase* URTSDKAssociatedModDefinitionBase::GetAssociatedMod()
{
#if !WITH_EDITORONLY_DATA
	return AssociatedModPtr;
#else
	return AssociatedMod.Get();
#endif	
}

FName URTSDKAssociatedModDefinitionBase::GetAllowedAssociatedModType() const
{
	return FName();
}
