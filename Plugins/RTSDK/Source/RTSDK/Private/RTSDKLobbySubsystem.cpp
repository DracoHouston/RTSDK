// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKLobbySubsystem.h"
#include "RTSDKModManager.h"
#include "LoadingProcessTask.h"

void URTSDKLobbySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ModManager = GEngine->GetEngineSubsystem<URTSDKModManager>();
}

bool URTSDKLobbySubsystem::CreateNewLobbyPIE(UWorld* CallerWorld, FString inGameModDevName, const TArray<FString>& inFactionModDevNames, FString inMapModDevName, const TArray<FString>& inMutatorDevNames)
{
	CurrentLoadingProcessTask = NewObject<ULoadingProcessTask>();
	CurrentLoadingProcessTask->SetShowLoadingScreenReason(TEXT("LOADING MODS"));
	ModManager->OnActiveModsReady.AddDynamic(this, &URTSDKLobbySubsystem::OnModsFullyActivated);
	return ModManager->ActivateModsByName(CallerWorld, inGameModDevName, inFactionModDevNames, inMapModDevName, inMutatorDevNames).IsValid();
}

void URTSDKLobbySubsystem::DestroyLobbyPIE(UWorld* CallerWorld)
{
	ModManager->DeactivateMods(CallerWorld);
}

bool URTSDKLobbySubsystem::CreateNewLobby(FName inGameModDevName, const TArray<FName>& inFactionModDevNames, FName inMapModDevName, const TArray<FName>& inMutatorDevNames)
{
	return false;
}

bool URTSDKLobbySubsystem::SetGameMod(FName inModDevName)
{
	return false;
}

bool URTSDKLobbySubsystem::SetFactionMods(const TArray<FName>& inModDevNames)
{
	return false;
}

bool URTSDKLobbySubsystem::AddFactionMod(FName inModDevName)
{
	return false;
}

bool URTSDKLobbySubsystem::RemoveFactionMod(FName inModDevName)
{
	return false;
}

bool URTSDKLobbySubsystem::SetMapMod(FName inModDevName)
{
	return false;
}

bool URTSDKLobbySubsystem::SetMutators(const TArray<FName>& inModDevNames)
{
	return false;
}

bool URTSDKLobbySubsystem::AddMutator(FName inModDevName)
{
	return false;
}

bool URTSDKLobbySubsystem::RemoveMutator(FName inModDevName)
{
	return false;
}

void URTSDKLobbySubsystem::LaunchGame()
{
}

bool URTSDKLobbySubsystem::IsFullyLoaded()
{
	return CurrentLoadingProcessTask == nullptr;
}

void URTSDKLobbySubsystem::OnModsFullyActivated(UWorld* WorldContext, FRTSDKActiveModsInfo inActiveModsInfo)
{
	ModManager->OnActiveModsReady.RemoveAll(this);
	CurrentLoadingProcessTask = nullptr;
}
