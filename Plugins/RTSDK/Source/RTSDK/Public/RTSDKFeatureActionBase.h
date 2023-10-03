// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "RTSDKFeatureActionBase.generated.h"

class UMassProcessor;

/**
 * Registers a Faction Mod with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureActionBase : public UGameFeatureAction
{
	GENERATED_BODY()

public:

	//UGameFeatureAction
	virtual void OnGameFeatureRegistering() override {}
	virtual void OnGameFeatureUnregistering() override {}
	virtual void OnGameFeatureLoading() override {}
	virtual void OnGameFeatureActivating() override {}
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override {}
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override {}
	//~UGameFeatureAction

	UFUNCTION()
		virtual void OnGameFeatureActivating(UWorld* WorldContext) {}

	UFUNCTION()
		virtual void OnGameFeatureDeactivating(UWorld* WorldContext) {}
};
