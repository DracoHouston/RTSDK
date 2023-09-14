// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturesProjectPolicies.h"
#include "RTSDKGameFeaturePolicy.generated.h"

/**
 * Game Feature policy that initializes the mod manager after game features have registered.
 */
UCLASS()
class RTSDK_API URTSDKGameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
	
public:
	//UGameFeaturesProjectPolicies
	virtual void InitGameFeatureManager() override;
	//~UGameFeaturesProjectPolicies
};
