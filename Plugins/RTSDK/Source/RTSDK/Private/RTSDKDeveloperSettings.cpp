// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKDeveloperSettings.h"
#include "RTSDKSimState.h"
#include "RTSDKTeamState.h"
#include "RTSDKForceState.h"
#include "RTSDKCommanderState.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RTSDKGameUserSettings.h"
#include "CommonActivatableWidget.h"

URTSDKDeveloperSettings::URTSDKDeveloperSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TargetSimFramesPerSecond = 32;
	MaxFramesPerGameThreadTick = 32;
	MinimumNetTurnDuration = 0.25;
	MinimumLANTurnDuration = 0.125;
	LockstepTimeoutTurnCount = 4;
	MaxTimescale = 10.0;
	LockstepTurnDurationTolerance = 0.015625;
	SimStateClass = ARTSDKSimStateServerClientLockstep::StaticClass();
	TeamStateClass = ARTSDKTeamStateServerClientLockstep::StaticClass();
	ForceStateClass = ARTSDKForceStateServerClientLockstep::StaticClass();
	CommanderStateClass = ARTSDKCommanderStateServerClientLockstep::StaticClass();
	SharedUserSettingsClass = URTSDKSharedUserSettings::StaticClass();
	DefaultInputProfile = FRTSDKInputSettingsProfile();
	if (GlobalMappingContext.IsValid())
	{
		FRTSDKSavedBindContext stratpawncontext(GlobalMappingContext.Get());

		DefaultInputProfile.PlayerBindContexts.Add(stratpawncontext);
	}
}

void URTSDKDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	DefaultInputProfile = FRTSDKInputSettingsProfile();
	if (GlobalMappingContext != nullptr)
	{
		FRTSDKSavedBindContext stratpawncontext(GlobalMappingContext.Get());

		DefaultInputProfile.PlayerBindContexts.Add(stratpawncontext);
	}
}
