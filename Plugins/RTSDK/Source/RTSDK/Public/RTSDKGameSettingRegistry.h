// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DataSource/GameSettingDataSourceDynamic.h" // IWYU pragma: keep
#include "GameSettingRegistry.h"
#include "Logging/LogMacros.h"
#include "Misc/AssertionMacros.h"
#include "UObject/UObjectGlobals.h"
#include "RTSDKGameSettingRegistry.generated.h"

class ULocalPlayer;
class URTSDKPlayerSettingsSubsystem;
class UObject;
class UGameSettingCollection;

DECLARE_LOG_CATEGORY_EXTERN(LogRTSDKGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(URTSDKLocalPlayer, GetSharedSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(URTSDKInputSettingsProfile, FunctionOrPropertyName)		\
	}))

/**
 * 
 */
UCLASS()
class URTSDKGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	URTSDKGameSettingRegistry();
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	//UGameSettingCollection* InitializeVideoSettings(ULyraLocalPlayer* InLocalPlayer);

	//UGameSettingCollection* InitializeAudioSettings(ULyraLocalPlayer* InLocalPlayer);
	//UGameSettingCollection* InitializeGameplaySettings(ULyraLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeInputSettings(ULocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> InputSettings;

	UPROPERTY()
		TObjectPtr<URTSDKPlayerSettingsSubsystem> PlayerSettingsSubsystem;
};
