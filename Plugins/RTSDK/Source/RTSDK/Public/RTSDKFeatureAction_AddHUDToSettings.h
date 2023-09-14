// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "RTSDKFeatureAction_AddHUDToSettings.generated.h"

class URTSDKHUDLayoutData;

/**
 * Registers a configurable, game mod associated HUD with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureAction_AddHUDToSettings : public UGameFeatureAction
{
	GENERATED_BODY()

public:

	//UGameFeatureAction
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureUnregistering() override;
	//~UGameFeatureAction

	/**
	* The internal name of the HUD, must be unique per game mod, not user facing
	*/
	UPROPERTY(EditAnywhere)
		FName HUDDevName;

	/**
	* Display name of the HUD
	*/
	UPROPERTY(EditAnywhere)
		FText HUDDisplayName;

	/**
	* A description of this HUD, for tooltips etc
	*/
	UPROPERTY(EditAnywhere)
		FText HUDDescriptionText;

	/**
	* The associated Game Mod devname, will show up under this game's settings
	*/
	UPROPERTY(EditAnywhere)
		FName AssociatedGameModDevName;

	/**
	* The parent HUD of this HUD, if any.
	*/
	UPROPERTY(EditAnywhere)
		FName ParentHUDDevName;

	/**
	* Whether this HUD is 'abstract', and only to be used via children, hidden in most contexts
	*/
	UPROPERTY(EditAnywhere)
		bool bIsAbstractHUD;

	/**
	* The HUD Layout Data asset that contains the initial settings.
	*/
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<URTSDKHUDLayoutData> HUDLayoutData;
};
