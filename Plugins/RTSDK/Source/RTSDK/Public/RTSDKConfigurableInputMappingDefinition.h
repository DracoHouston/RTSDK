// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKTypes.h"
#include "RTSDKConfigurableInputMappingDefinition.generated.h"

class URTSDKGameModDefinition;
class URTSDKModManager;
class UInputMappingContext;

/**
 * Runtime definition of a configurable HUD, for RTSDK Mod Manager.
 * Configurable HUDs are associated with a game mod and can be inherited by child game mods.
 * They are registered with the Mod Manager by a game feature using RTSDKFeatureAction_AddHudToSettings
 * and filling out then referencing a RTSDKHUDLayoutData data asset in the properties for the action.
 * They can therefore be added by any kind of mod or game feature, despite game mod association.
 */
UCLASS()
class RTSDK_API URTSDKConfigurableInputMappingDefinition : public UObject
{
	GENERATED_BODY()
	
public: 

	UFUNCTION()
		void Init(
			FName inDevName, 
			const FText& inDisplayName, 
			const FText& inDescriptionText, 
			FName inAssociatedGameModDevName,
			TSoftObjectPtr<UInputMappingContext> inMappingContextAsset);

	UFUNCTION()
		void BuildModDependencies(URTSDKModManager* inModManager);

	UFUNCTION()
		void BuildMod(URTSDKModManager* inModManager);

	/**
	* The internal name of the HUD, must be unique per game mod, not user facing
	*/
	UPROPERTY(transient)
		FName MappingDevName;

	/**
	* Display name of the HUD
	*/
	UPROPERTY(transient)
		FText MappingDisplayName;

	/**
	* A description of this HUD, for tooltips etc
	*/
	UPROPERTY(transient)
		FText MappingDescriptionText;

	/**
	* The associated Game Mod devname, will show up under this game's settings
	*/
	UPROPERTY(transient)
		FName AssociatedGameModDevName;

	UPROPERTY(transient)
		TObjectPtr<URTSDKGameModDefinition> AssociatedGameMod;

	UPROPERTY(transient)
		TSoftObjectPtr<UInputMappingContext> MappingContextAsset;

	UPROPERTY(transient)
		FRTSDKSavedBindContext MappingContextData;

	UPROPERTY(transient)
		bool bIsValid;
};
