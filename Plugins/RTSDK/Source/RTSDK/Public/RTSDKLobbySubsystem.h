// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RTSDKLobbySubsystem.generated.h"

class ULoadingProcessTask;
class URTSDKLobbySettingRegistry;

/**
 * Manager for the active 'lobby' within RTSDK, a pending or current game session.
 * handles lobby options and spinning up game sessions 
 */
UCLASS()
class RTSDK_API URTSDKLobbySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	* Sets the initial mods and initializes a new lobby
	*/
	UFUNCTION(BlueprintCallable)
	bool CreateNewLobby(FName inGameModDevName, const TArray<FName>& inFactionModDevNames, FName inMapModDevName, const TArray<FName>& inMutatorDevNames);
	
	/**
	* Sets the game mod, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool SetGameMod(FName inModDevName);

	/**
	* Sets the faction mods in bulk, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool SetFactionMods(const TArray<FName>& inModDevNames);

	/**
	* Adds a new faction mod to the lobby, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool AddFactionMod(FName inModDevName);

	/**
	* Removes an existing faction mod from the lobby, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveFactionMod(FName inModDevName);

	/**
	* Sets the map mod, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool SetMapMod(FName inModDevName);

	/**
	* Sets the mutators in bulk, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool SetMutators(const TArray<FName>& inModDevNames);
	
	/**
	* Adds a new mutator to the lobby, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool AddMutator(FName inModDevName);
	
	/**
	* Removes an existing mutator from the lobby, usable while staging the lobby, not while playing.
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveMutator(FName inModDevName);

	/**
	* Launch a staged lobby, loading and activating necessary game features for the chosen mods, and loading into the game world
	*/
	UFUNCTION(BlueprintCallable)
	void LaunchGame();

protected:
	/**
	* This is the current 'loading process task', if any. 
	* When this exists it causes the load screen to appear until unregistered.
	*/
	UPROPERTY()
		TObjectPtr<ULoadingProcessTask> CurrentLoadingProcessTask;

	/**
	* Lobby Settings Registry, holds the active lobby settings as GameSetting objects.
	* Valid for both the lobby UI and for game world level objects to query for initialization, like the game sim subsystem
	*/
	UPROPERTY()
		TObjectPtr<URTSDKLobbySettingRegistry> LobbySettingsRegistry;
};
