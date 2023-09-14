// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSDKPlayerControllerInterface.generated.h"

struct FRTSDKPlayerCommandReplicationInfo;
class ARTSDKCommanderStateBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSDKPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for RTSDK player controller methods. 
 * RTSDK will recognise any controller implementing this as a player.
 * See RTSDKPlayerController for an example implementation.
 */
class RTSDK_API IRTSDKPlayerControllerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
		virtual void SetReadyState(bool inIsReady) {}

	UFUNCTION()
		virtual void SendPlayerCommand(const FRTSDKPlayerCommandReplicationInfo& inCommand) {}

	UFUNCTION()
		virtual void FinishInputTurn(int32 inTurn, int32 inChecksum) {}

	UFUNCTION()
		virtual void RequestPause() {}

	UFUNCTION()
		virtual void RequestUnpause() {}

	UFUNCTION()
		virtual void RequestTimescale(const FFixed64& inTimescale) {}

	UFUNCTION()
		virtual ARTSDKCommanderStateBase* GetCommanderState() { return nullptr; }

	UFUNCTION()
		virtual void SetCommanderState(ARTSDKCommanderStateBase* inState) {}

	UFUNCTION()
		virtual bool GetWantsToBeReady() { return false; }
};
