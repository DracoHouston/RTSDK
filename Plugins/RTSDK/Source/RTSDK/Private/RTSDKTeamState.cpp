// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKTeamState.h"
#include "RTSDKForceState.h"
#include "RTSDKCommanderState.h"
#include "RTSDKSimState.h"
#include "RTSDKGameSimSubsystem.h"
#include "Net/UnrealNetwork.h"

ARTSDKTeamStateBase::ARTSDKTeamStateBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARTSDKTeamStateBase::Setup(const FRTSDKStateSetupInfo& inSetup, ARTSDKSimStateBase* inSimState, URTSDKGameSimSubsystem* inSimSubsystem)
{
    SimSubsystem = inSimSubsystem;
    SimState = inSimState;
    for (auto It = inSetup.OptionsMap.CreateConstIterator(); It; ++It)
    {
        if (It->Key == TEXT("Name"))
        {
            SetDisplayName(FText::FromString(It->Value));
        }
    }
}

ARTSDKTeamStateSPOnly::ARTSDKTeamStateSPOnly(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = false;
}

FText ARTSDKTeamStateSPOnly::GetDisplayName()
{
    return DisplayName;
}

void ARTSDKTeamStateSPOnly::SetDisplayName(const FText& inName)
{
    DisplayName = inName;
}

TArray<ARTSDKForceStateBase*> ARTSDKTeamStateSPOnly::GetForces()
{
    return Forces;
}

void ARTSDKTeamStateSPOnly::SetForces(TArray<ARTSDKForceStateBase*> inForces)
{
    Forces = inForces;
}

int32 ARTSDKTeamStateSPOnly::AddForce(ARTSDKForceStateBase* inForce)
{
    return Forces.Add(inForce);
}

int32 ARTSDKTeamStateSPOnly::GetForceCount()
{
    return Forces.Num();
}

ARTSDKTeamStateServerClientLockstep::ARTSDKTeamStateServerClientLockstep(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

FText ARTSDKTeamStateServerClientLockstep::GetDisplayName()
{
    return DisplayName;
}

void ARTSDKTeamStateServerClientLockstep::SetDisplayName(const FText& inName)
{
    DisplayName = inName;
}

TArray<ARTSDKForceStateBase*> ARTSDKTeamStateServerClientLockstep::GetForces()
{
    return Forces;
}

void ARTSDKTeamStateServerClientLockstep::SetForces(TArray<ARTSDKForceStateBase*> inForces)
{
    Forces = inForces;
}

int32 ARTSDKTeamStateServerClientLockstep::AddForce(ARTSDKForceStateBase* inForce)
{
    return Forces.Add(inForce);
}

int32 ARTSDKTeamStateServerClientLockstep::GetForceCount()
{
    return Forces.Num();
}

void ARTSDKTeamStateServerClientLockstep::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSDKTeamStateServerClientLockstep, Forces);
    DOREPLIFETIME(ARTSDKTeamStateServerClientLockstep, DisplayName);
}

ARTSDKTeamStateServerClientCurves::ARTSDKTeamStateServerClientCurves(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

FText ARTSDKTeamStateServerClientCurves::GetDisplayName()
{
    return DisplayName;
}

void ARTSDKTeamStateServerClientCurves::SetDisplayName(const FText& inName)
{
    DisplayName = inName;
}

TArray<ARTSDKForceStateBase*> ARTSDKTeamStateServerClientCurves::GetForces()
{
    return Forces;
}

void ARTSDKTeamStateServerClientCurves::SetForces(TArray<ARTSDKForceStateBase*> inForces)
{
    Forces = inForces;
}

int32 ARTSDKTeamStateServerClientCurves::AddForce(ARTSDKForceStateBase* inForce)
{
    return Forces.Add(inForce);
}

int32 ARTSDKTeamStateServerClientCurves::GetForceCount()
{
    return Forces.Num();
}

void ARTSDKTeamStateServerClientCurves::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSDKTeamStateServerClientCurves, Forces);
    DOREPLIFETIME(ARTSDKTeamStateServerClientCurves, DisplayName);
}