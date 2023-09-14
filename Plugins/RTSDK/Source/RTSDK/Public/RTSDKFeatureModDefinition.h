// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKFeatureModDefinition.generated.h"

class URTSDKModManager;
class URTSDKGameFeatureData;

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKFeatureModDefinition : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void Init(const URTSDKGameFeatureData* inData);

	UFUNCTION()
		TArray<FString> LoadMod();

	UPROPERTY(transient)
		FText DisplayName;

	UPROPERTY(transient)
		FName DevName;

	UPROPERTY(transient)
		FString GameFeatureName;

	UPROPERTY(transient)
		FString GameFeatureURL;

	UPROPERTY(transient)
		bool bIsValid;

	UPROPERTY(transient)
		bool bIsFullyLoaded;

	UPROPERTY(transient)
		bool bIsActivated;

};
