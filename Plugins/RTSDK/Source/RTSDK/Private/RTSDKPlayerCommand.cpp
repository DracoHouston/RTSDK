// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKPlayerCommand.h"
#include "RTSDKCommanderState.h"
#include "RTSDKTeamState.h"
#include "RTSDKForceState.h"
#include "RTSDKSimState.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKFragments.h"
#include "MassEntityView.h"

void URTSDKPlayerCommandBase::SetAll(ARTSDKCommanderStateBase* inCommander, URTSDKGameSimSubsystem* inSimSubsystem, const FRTSDKPlayerCommandReplicationInfo& Info)
{
	TargetLocations = Info.TargetLocations;
	TargetRotations = Info.TargetRotations;
	TargetUnitIDs = Info.TargetUnitIDs;
	UnitIDs = Info.UnitIDs;
	TargetUnitTypes = Info.TargetUnitTypes;
	Commander = inCommander;
	Force = Commander->GetForce();
	Team = Force->GetTeam();
	SimSubsystem = inSimSubsystem;
	SimState = SimSubsystem->GetSimState();
}

void URTSDKPawnMoveInputPlayerCommand::Execute()
{
//	const FRTSDKRegisteredUnitInfo* unitinfo = SimSubsystem->GetUnitInfoByID(UnitIDs[0]);
//	FMassEntityView unitentity(*SimSubsystem->GetEntityManager(), unitinfo->UnitHandle);
//	FRTSMovementInputFragment& moveinput = unitentity.GetFragmentData<FRTSMovementInputFragment>();
//#if RTSDK_USE_FIXED_POINT
//	moveinput.Input = TargetLocations[0];
//#else
//	moveinput.Input.X = (double)TargetLocations[0].X;
//	moveinput.Input.Y = (double)TargetLocations[0].Y;
//	moveinput.Input.Z = (double)TargetLocations[0].Z;
//#endif
}
