// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "RTSDKFeatureAction_AddMutator.generated.h"

/**
 * Registers a Game Mod with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureAction_AddMutator : public UGameFeatureAction
{
	GENERATED_BODY()
	
public:

	//UGameFeatureAction
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureUnregistering() override;
	//~UGameFeatureAction

	UPROPERTY(EditAnywhere)
		FName ModDevName;

	UPROPERTY(EditAnywhere)
		FText ModDisplayName;

	/**
	* The name of the primary Game Feature Plugin this Mutator resides in
	*/
	UPROPERTY(EditAnywhere)
		FString GameFeatureName;

	/**
	* Names of any additional Game Feature Plugins this Mutator wishes to enable.
	*/
	UPROPERTY(EditAnywhere)
		TArray<FString> AdditionalGameFeaturesToEnable;

	/**
	* Dev name of the associated Game Mod, if any. If left blank, will be considered valid for any Game Mod.
	*/
	UPROPERTY(EditAnywhere)
		FName AssociatedGameModDevName;
};
