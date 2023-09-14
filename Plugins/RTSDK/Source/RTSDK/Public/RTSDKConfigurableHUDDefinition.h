// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKConfigurableHUDDefinition.generated.h"

class URTSDKGameModDefinition;
class URTSDKModManager;
struct FRTSDKInitialHUDElementSettings;

/**
 * Runtime definition of a configurable HUD, for RTSDK Mod Manager.
 * Configurable HUDs are associated with a game mod and can be inherited by child game mods.
 * They are registered with the Mod Manager by a game feature using RTSDKFeatureAction_AddHudToSettings
 * and filling out then referencing a RTSDKHUDLayoutData data asset in the properties for the action.
 * They can therefore be added by any kind of mod or game feature, despite game mod association.
 */
UCLASS()
class RTSDK_API URTSDKConfigurableHUDDefinition : public UObject
{
	GENERATED_BODY()
	
public: 

	UFUNCTION()
		void Init(
			FName inDevName, 
			const FText& inDisplayName, 
			const FText& inDescriptionText, 
			FName inAssociatedGameModDevName, 
			FName inParentHUDDevName,
			TSoftClassPtr<UUserWidget> inWidgetClass, 
			TArray<FRTSDKInitialHUDElementSettings> inElements,
			bool inbIsAbstractHUD);

	UFUNCTION()
		void BuildModDependencies(URTSDKModManager* inModManager);

	UFUNCTION()
		void BuildMod(URTSDKModManager* inModManager);

	UPROPERTY(transient)
		TArray<FRTSDKInitialHUDElementSettings> Elements;

	UPROPERTY(transient)
		TArray<FRTSDKInitialHUDElementSettings> CombinedElements;

	/**
	* The internal name of the HUD, must be unique per game mod, not user facing
	*/
	UPROPERTY(transient)
		FName HUDDevName;

	/**
	* Display name of the HUD
	*/
	UPROPERTY(transient)
		FText HUDDisplayName;

	/**
	* A description of this HUD, for tooltips etc
	*/
	UPROPERTY(transient)
		FText HUDDescriptionText;

	/**
	* The widget class for this HUD
	*/
	UPROPERTY(transient)
		TSoftClassPtr<UUserWidget> WidgetClass;

	/**
	* The associated Game Mod devname, will show up under this game's settings
	*/
	UPROPERTY(transient)
		FName AssociatedGameModDevName;

	UPROPERTY(transient)
		TObjectPtr<URTSDKGameModDefinition> AssociatedGameMod;

	UPROPERTY(transient)
		FName ParentHUDDevName;

	UPROPERTY(transient)
		TObjectPtr<URTSDKConfigurableHUDDefinition> ParentHUD;

	UPROPERTY(transient)
		bool bIsValid;

	UPROPERTY(transient)
		bool bIsAbstractHUD;
};
