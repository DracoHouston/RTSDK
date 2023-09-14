// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "RTSDKFeatureAction_AddFactionMod.generated.h"

class URTSDKFactionDefinition;

/**
 * Registers a Faction Mod with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureAction_AddFactionMod : public UGameFeatureAction
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
		FName ParentFactionModDevName;

	UPROPERTY(EditAnywhere)
		FName AssociatedGameModDevName;

	UPROPERTY(EditAnywhere)
		TSoftClassPtr<URTSDKFactionDefinition> FactionClass;
};
