// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKLobbySubsystem.h"
#include "LoadingProcessTask.h"

void URTSDKLobbySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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
