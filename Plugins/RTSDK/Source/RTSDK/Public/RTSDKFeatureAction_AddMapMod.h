// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "RTSDKFeatureAction_AddMapMod.generated.h"

/**
 * Registers a Map Mod with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureAction_AddMapMod : public UGameFeatureAction
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

	UPROPERTY(EditAnywhere)
		FString GameFeatureName;

	UPROPERTY(EditAnywhere)
		bool bIsAbstractMod;

	UPROPERTY(EditAnywhere)
		FName ParentMapModDevName;

	UPROPERTY(EditAnywhere)
		FName AssociatedGameModDevName;

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Map"))
		FPrimaryAssetId AssociatedLevel;
};
