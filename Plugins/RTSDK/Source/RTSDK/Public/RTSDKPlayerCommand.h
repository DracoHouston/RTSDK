// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "RTSDKConstants.h"
#include "FixedPointTypes.h"
#include "RTSDKPlayerCommand.generated.h"

class ARTSDKCommanderStateBase;
class ARTSDKSimStateBase;
class ARTSDKTeamStateBase;
class ARTSDKForceStateBase;
class URTSDKGameSimSubsystem;
class URTSDKUnitDefinition;

USTRUCT()
struct RTSDK_API FRTSDKPlayerCommandReplicationInfo
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<uint32> UnitIDs;

		UPROPERTY()
		TSubclassOf<URTSDKPlayerCommandBase> Class;

	UPROPERTY()
		TArray<FFixedVector64> TargetLocations;

	UPROPERTY()
		TArray<FFixedRotator64> TargetRotations;

	UPROPERTY()
		TArray<uint32> TargetUnitIDs;

	UPROPERTY()
		TArray<TSubclassOf<URTSDKUnitDefinition>> TargetUnitTypes;
};

/**
* Abstract base class for "Player Commands" in RTSDK
* Created by the RTSDK Game Sim Subsystem when a confirmed command comes in.
* Players send commands via RPC, the arguments for which are:
* Target Locations - An array of vector values. Typically signify locations.
* Target Rotations - An array of rotator values. Typically signify rotations.
* Target Unit IDs - An array of integer values. Typically signify Unit IDs.
* Target Unit Types - An array of FName values. Typically signify Unit Types.
* These are available along with the Player that issued the command, as a PlayerState, as members.
* When the command is ready to execute the virtual function Execute is called.
* Commands must implement their own Execute method for the command to do anything.
*/
UCLASS(abstract)
class RTSDK_API URTSDKPlayerCommandBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void Execute() {}

	void SetAll(ARTSDKCommanderStateBase* inCommander, URTSDKGameSimSubsystem* inSimSubsystem, const FRTSDKPlayerCommandReplicationInfo& Info);	

protected:
	TWeakObjectPtr<ARTSDKCommanderStateBase> Commander;
	TWeakObjectPtr<ARTSDKTeamStateBase> Team;
	TWeakObjectPtr<ARTSDKForceStateBase> Force;
	TWeakObjectPtr<ARTSDKSimStateBase> SimState;
	TWeakObjectPtr<URTSDKGameSimSubsystem> SimSubsystem;
	TArray<uint32> UnitIDs;
	TArray<FFixedVector64> TargetLocations;
	TArray<FFixedRotator64> TargetRotations;
	TArray<uint32> TargetUnitIDs;
	TArray<TSubclassOf<URTSDKUnitDefinition>> TargetUnitTypes;
};

UCLASS()
class RTSDK_API URTSDKPawnMoveInputPlayerCommand : public URTSDKPlayerCommandBase
{
	GENERATED_BODY()
public:
	virtual void Execute() override;
};
