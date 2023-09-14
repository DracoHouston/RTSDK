// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FixedPointTypes.h"
#include "RTSDKTypes.h"
#include "RTSDKDeveloperSettings.generated.h"

class ARTSDKSimStateBase;
class ARTSDKTeamStateBase;
class ARTSDKForceStateBase;
class ARTSDKCommanderStateBase;
class UInputAction;
class UInputMappingContext;
class URTSDKSharedUserSettingsBase;
class UCommonActivatableWidget;

/**
 * 
 */
UCLASS(config = RTSDK, defaultconfig, DisplayName = "Real Time Strategy Dev Kit", AutoExpandCategories = "RTSDK")
class RTSDK_API URTSDKDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	URTSDKDeveloperSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

public:
	//The number of sim frames that will be needed to simulate a second of gameplay in unscaled real time
	//Timestep is derived from this. Default 32 sim frames per second. 
	//Note: Because the game thread ticks at some capped rate (usually 20, 30, 60, 144 etc) values above the cap
	//will advance multiple sim frames per tick. Values below the cap may have frames where the sim does not advance frame at all.
	//Because the sim is fixed timestep, timescales above 1 require more frames per real time second to simulate the frames at scaled time.
	//Replay seeking also requires a lot of frames to catch up to the point being seeked to. A lower FPS, and resulting longer timestep
	//reduces the frames required to catch up some multi-second amount of time. 
	//By default, this is 32, timestep 0.03125 seconds per frame
	UPROPERTY(EditDefaultsOnly, config, Category="RTSDK")
		int32 TargetSimFramesPerSecond;

	//Maximum number of sim frames that may execute in a single game thread tick. Default is 32, 1 second of default target sim fps
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		int32 MaxFramesPerGameThreadTick;

	//Minimum turn duration for internet matches, in seconds. Default 0.25s, or 250ms
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		FFixed64 MinimumNetTurnDuration;

	//Minimum turn duration for LAN matches, in seconds. Default 0.125s, or 125ms
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		FFixed64 MinimumLANTurnDuration;

	//Maximum allowed timescale for players. Below 1 is time dilation, above 1 is time compression. Default 10.0
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		FFixed64 MaxTimescale;

	//Maximum allowed delta between the current turn duration and the desired turn duration before duration is changed, in seconds. Default 0.015625
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		FFixed64 LockstepTurnDurationTolerance;

	//Number of turns server may get ahead of clients before stopping the sim to wait for them to catch up. Default 4.
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		int32 LockstepTimeoutTurnCount;

	//Number of turns ahead input received in a completed turn is queued to actually execute. Default 2
	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		int32 LockstepInputTurnDelay;

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		TSubclassOf<ARTSDKSimStateBase> SimStateClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		TSubclassOf<ARTSDKTeamStateBase> TeamStateClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		TSubclassOf<ARTSDKForceStateBase> ForceStateClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDK")
		TSubclassOf<ARTSDKCommanderStateBase> CommanderStateClass;

	//The global mapping context, for mappable binds that are always active.
	UPROPERTY(EditDefaultsOnly, config, Category = "Input")
		TSoftObjectPtr<UInputMappingContext> GlobalMappingContext;

	UPROPERTY(EditDefaultsOnly, config, Category = "User Settings")
		TSubclassOf<URTSDKSharedUserSettingsBase> SharedUserSettingsClass;

	//TODO: do we really need this?
	//Generated at design time, this can be copied and pasted into shared user settings blueprints class defaults. for initial profiles for new users
	//Changing any input mapping context option in these settings, in the Input category, triggers a refresh.
	UPROPERTY(VisibleAnywhere, config, Category = "Input")
		FRTSDKInputSettingsProfile DefaultInputProfile;
};
