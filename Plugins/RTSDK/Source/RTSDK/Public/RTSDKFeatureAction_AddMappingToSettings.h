// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "RTSDKFeatureAction_AddMappingToSettings.generated.h"

class UInputMappingContext;


/**
 * Registers a configurable, game mod associated HUD with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureAction_AddMappingToSettings : public UGameFeatureAction
{
	GENERATED_BODY()

public:

	//UGameFeatureAction
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureUnregistering() override;
	//~UGameFeatureAction

	/**
	* The internal name of the InputMapping, must be unique, not user facing
	*/
	UPROPERTY(EditAnywhere)
		FName MappingDevName;

	/**
	* Display name of the HUD
	*/
	UPROPERTY(EditAnywhere)
		FText MappingDisplayName;

	/**
	* A description of this HUD, for tooltips etc
	*/
	UPROPERTY(EditAnywhere)
		FText MappingDescriptionText;

	/**
	* The associated Game Mod devname, will show up under this game's settings
	*/
	UPROPERTY(EditAnywhere)
		FName AssociatedGameModDevName;

	/**
	* The Enhanced Input 'Input Mapping Context' data asset containing the mapping.
	*/
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UInputMappingContext> MappingContextAsset;
};
