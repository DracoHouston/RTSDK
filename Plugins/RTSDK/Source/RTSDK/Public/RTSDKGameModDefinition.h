// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKModDefinitionBase.h"
#include "RTSDKGameModDefinition.generated.h"

class URTSDKModManager;
class URTSDKGameFeatureData;
class URTSDKFeatureModDefinition;
class URTSDKMapModDefinition;
class URTSDKFactionModDefinition;
class URTSDKConfigurableHUDDefinition;

/**
 * A definition of a Game Mod, logic and content for an implementation of
 * a game in RTSDK.
 * Map and Faction Mods are dependent on Game Mods.
 * Game Mods can depend on other Game Mods, but only 1 is the active Game at a time.
 * Eg. Campaign Game and Multiplayer Game derived from Base Game
 * And some user Game Mod derived from Multiplayer Game as the actual active game.
 * or for this case: Base->Multiplayer->User
 * Map Mods will accept children of their designated Game Mod, as will Faction Mods.
 * 
 * Populates the Mod Manager list of Game Mods.
 */
UCLASS()
class RTSDK_API URTSDKGameModDefinition : public URTSDKUnitProvidingModDefinitionBase
{
	GENERATED_BODY()
	
public:

	virtual TArray<IRTSDKDataEditorTreeNodeInterface*> GetChildNodeObjectsForDataEditorTree() override;

	//virtual void Init(const URTSDKGameFeatureData* inData) override;
	virtual void BuildMod(URTSDKModManager* inModManager) override;

	virtual FName GetModType() const override;

	UPROPERTY(transient)
		TArray<URTSDKMapModDefinition*> ValidMapMods;

	UPROPERTY(transient)
		TArray<URTSDKFactionModDefinition*> ValidFactionMods;

	/*UPROPERTY(transient)
		TArray<URTSDKConfigurableHUDDefinition*> ValidConfigurableHUDs;*/

	UPROPERTY(transient)
		TSubclassOf<UUserWidget> GameMenuWidgetClass;
};
