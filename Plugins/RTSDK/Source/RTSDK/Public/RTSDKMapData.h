// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RTSDKMapData.generated.h"

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKScenarioCommander
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsPlayer;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKScenarioForce
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FRTSDKScenarioCommander> Commanders;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKScenarioTeam
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FRTSDKScenarioForce> Forces;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKScenarioSetup
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FRTSDKScenarioTeam> Teams;
};

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKMapData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSoftObjectPtr<ULevel> MapAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsScenario;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MinPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FRTSDKScenarioSetup ScenarioSetup;
};
