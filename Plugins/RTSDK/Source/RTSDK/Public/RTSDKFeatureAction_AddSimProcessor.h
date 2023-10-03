// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKFeatureActionBase.h"
#include "RTSDKFeatureAction_AddSimProcessor.generated.h"

class UMassProcessor;

/**
 * Registers a Faction Mod with the Mod Manager
 */
UCLASS()
class RTSDK_API URTSDKFeatureAction_AddSimProcessor : public URTSDKFeatureActionBase
{
	GENERATED_BODY()

public:

	//UGameFeatureAction
	virtual void OnGameFeatureActivating(UWorld* WorldContext) override;
	virtual void OnGameFeatureDeactivating(UWorld* WorldContext) override;
	//~UGameFeatureAction

	UPROPERTY(EditAnywhere)
		TArray<TSoftClassPtr<UMassProcessor>> ProcessorClasses;
};
