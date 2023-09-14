// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataSource/GameSettingDataSourceDynamic.h" // IWYU pragma: keep
#include "GameSettingRegistry.h"
#include "Logging/LogMacros.h"
#include "Misc/AssertionMacros.h"
#include "UObject/UObjectGlobals.h"
#include "RTSDKLobbySettingRegistry.generated.h"

class ULocalPlayer;
class URTSDKLobbySubsystem;
class UObject;
class UGameSettingCollection;

DECLARE_LOG_CATEGORY_EXTERN(LogRTSDKLobbySettingRegistry, Log, Log);

//#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
//	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
//		GET_FUNCTION_NAME_STRING_CHECKED(URTSDKLocalPlayer, GetSharedSettings),				\
//		GET_FUNCTION_NAME_STRING_CHECKED(URTSDKInputSettingsProfile, FunctionOrPropertyName)		\
//	}))

/**
 * 
 */
UCLASS()
class URTSDKLobbySettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	URTSDKLobbySettingRegistry();
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	//UGameSettingCollection* InitializeVideoSettings(ULyraLocalPlayer* InLocalPlayer);

	//UGameSettingCollection* InitializeAudioSettings(ULyraLocalPlayer* InLocalPlayer);
	//UGameSettingCollection* InitializeGameplaySettings(ULyraLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeGameSettings(ULocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeFactionSettings(ULocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeMapSettings(ULocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeMutatorSettings(ULocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> FactionSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MapSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MutatorSettings;

	UPROPERTY()
		TObjectPtr<URTSDKLobbySubsystem> LobbySubsystem;
};
