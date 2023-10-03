// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKModDefinitionBase.h"
#include "RTSDKFeatureModDefinition.generated.h"

class URTSDKModManager;
class URTSDKGameFeatureData;

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKFeatureModDefinition : public URTSDKModDefinitionBase
{
	GENERATED_BODY()
	
public:
	virtual FName GetModType() const override;
	//virtual void Init(const URTSDKGameFeatureData* inData) override;
	//TODO: add designer set dev settings to feature mods, allow mods that depend on them to specify values for the settings
	//mod manager needs to resolve these as mods activate, the game or map gets highest priority in setting a setting, then factions then mutators
	//maybe make them generic so the games/maps/factions can declare their own for themselves that they inherit from parents?
};
