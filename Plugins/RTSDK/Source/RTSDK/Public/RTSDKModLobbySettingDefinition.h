// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKModLobbySettingDefinition.generated.h"

class URTSDKGameModDefinition;

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKModLobbySettingDefinition : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void Init(
			FName inDevName, 
			FText inDisplayName, 
			FString inGameFeatureName);

	UPROPERTY(transient)
		FText DisplayName;

	UPROPERTY(transient)
		FName DevName;

	UPROPERTY(transient)
		FString GameFeatureName;

};
