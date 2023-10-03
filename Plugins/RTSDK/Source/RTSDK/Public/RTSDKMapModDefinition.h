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
class RTSDK_API URTSDKMapModDefinition : public URTSDKAssociatedModDefinitionBase
{
	GENERATED_BODY()

public:

	//virtual void Init(const URTSDKGameFeatureData* inData) override;

	virtual FName GetModType() const override;
	virtual FName GetAllowedAssociatedModType() const override;

	UPROPERTY(EditDefaultsOnly)
		FPrimaryAssetId AssociatedLevel;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> GameMenuWidgetClass;
};
