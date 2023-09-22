// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKModDefinitionBase.h"
#include "RTSDKMapModDefinition.generated.h"

class URTSDKModManager;
class URTSDKGameFeatureData;
class URTSDKGameModDefinition; 
class URTSDKFeatureModDefinition;

/**
 * A definition of a Map Mod, logic and content for a world/level
 * Map mods are the entry point of the Mod system, which indicate what
 * Game Mods can be used, and so what Faction Mods can be used.
 * In addition to this, optionally, it can indicate dependence on Faction Mods 
 * and restrict choices to those factions. 
 * When starting a game instance and world the options chosen are passed by URL,
 * and validated against this definition.
 * 
 * Populates the Mod Manager list of Map Mods.
 */
UCLASS()
class RTSDK_API URTSDKMapModDefinition : public URTSDKModDefinitionBase
{
	GENERATED_BODY()

public:

	/*UFUNCTION()
		void Init(const URTSDKGameFeatureData* inData);

	UFUNCTION()
		void BuildModDependencies(URTSDKModManager* inModManager);
	
	UFUNCTION()
		void BuildMod(URTSDKModManager* inModManager);

	UFUNCTION()
		TArray<FString> LoadMod();

	UPROPERTY(transient)
		FText ModDisplayName;

	UPROPERTY(transient)
		FName ModDevName;

	UPROPERTY(transient)
		FString GameFeatureName;

	UPROPERTY(transient)
		FString GameFeatureURL;

	UPROPERTY(transient)
		TArray<FName> FeatureDependencyNames;

	UPROPERTY(transient)
		TArray<URTSDKFeatureModDefinition*> FeatureDependencies;

	UPROPERTY(transient)
		TArray<FString> CombinedGameFeatureURLs;

	UPROPERTY(transient)
		FName ParentMapModName;

	UPROPERTY(transient)
		TObjectPtr<URTSDKMapModDefinition> ParentMapMod;*/

	UPROPERTY(transient)
		FName AssociatedGameModName;

	UPROPERTY(transient)
		TObjectPtr<URTSDKGameModDefinition> AssociatedGameMod;

	UPROPERTY(transient)
		FPrimaryAssetId AssociatedLevel;

	UPROPERTY(transient)
		TSubclassOf<UUserWidget> GameMenuWidgetClass;

	/*UPROPERTY(transient)
		bool bIsValid;

	UPROPERTY(transient)
		bool bIsAbstractMod;

	UPROPERTY(transient)
		bool bIsFullyLoaded;

	UPROPERTY(transient)
		bool bIsActivated;*/
};
