// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKModDefinitionBase.h"
#include "RTSDKFactionModDefinition.generated.h"

class URTSDKGameModDefinition;
class URTSDKFactionDefinition;
class URTSDKFeatureModDefinition;

/**
 * Runtime definition of a Faction Mod, logic and content for a set of units.
 * Associated with a Game Mod, will work with any Game Mod derived from 
 * the designated required Game Mod.
 * Faction Mods may depend on another Faction Mod, allowing for modifications
 * from a base. Eg you might have the base Faction Mod, with a campaign and
 * multiplayer faction mod. The multiplayer faction mod can in turn be used
 * as a base for a user mod that further changes the faction.
 * 
 * Unless faction choice is restricted by a Map Mod, Faction Mods compatible with the
 * active Game Mod can be picked.
 * 
 * Populates the Mod Manager list of Faction Mods.
 */
UCLASS()
class RTSDK_API URTSDKFactionModDefinition : public URTSDKAssociatedModDefinitionBase
{
	GENERATED_BODY()
	
public:

	virtual void Init(const URTSDKGameFeatureData* inData) override;

	UPROPERTY(transient)
		TSoftClassPtr<URTSDKFactionDefinition> FactionClass;

	UPROPERTY(transient)
		TSubclassOf<UUserWidget> GameMenuWidgetClass;

};
