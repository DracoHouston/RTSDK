// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKForceState.h"
#include "RTSDKSimState.h"
#include "RTSDKTeamState.h"
#include "RTSDKCommanderState.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKLaunchOptionsHelpers.h"
#include "Net/UnrealNetwork.h"

ARTSDKForceStateBase::ARTSDKForceStateBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARTSDKForceStateBase::Setup(const FRTSDKStateSetupInfo& inSetup, ARTSDKSimStateBase* inSimState, URTSDKGameSimSubsystem* inSimSubsystem)
{
    SimSubsystem = inSimSubsystem;
    SimState = inSimState;
    for (auto It = inSetup.OptionsMap.CreateConstIterator(); It; ++It)
    {
        if (It->Key == TEXT("Name"))
        {
            SetDisplayName(FText::FromString(It->Value));
        }
        if (It->Key == TEXT("Team"))
        {
            int32 idx = FCString::Atoi(*It->Value);
            ARTSDKTeamStateBase* team = SimState->GetTeam(idx);
            if (team != nullptr)
            {
                SetTeam(team);
            }
        }
    }
}

ARTSDKForceStateSPOnly::ARTSDKForceStateSPOnly(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = false;
}

FText ARTSDKForceStateSPOnly::GetDisplayName()
{
    return DisplayName;
}

void ARTSDKForceStateSPOnly::SetDisplayName(const FText& inName)
{
    DisplayName = inName;
}

ARTSDKTeamStateBase* ARTSDKForceStateSPOnly::GetTeam()
{
    return Team.Get();
}

void ARTSDKForceStateSPOnly::SetTeam(ARTSDKTeamStateBase* inTeam)
{
    Team = inTeam;
}

TArray<ARTSDKCommanderStateBase*> ARTSDKForceStateSPOnly::GetCommanders()
{
    return Commanders;
}

ARTSDKCommanderStateBase* ARTSDKForceStateSPOnly::GetCommanderByIndex(int32 CommanderIndex)
{
    if (CommanderIndex < Commanders.Num())
    {
        return Commanders[CommanderIndex];
    }
    return nullptr;
}

void ARTSDKForceStateSPOnly::SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders)
{
    Commanders = inCommanders;
}

int32 ARTSDKForceStateSPOnly::AddCommander(ARTSDKCommanderStateBase* inCommander)
{
    return Commanders.Add(inCommander);
}

int32 ARTSDKForceStateSPOnly::GetCommanderCount()
{
    return Commanders.Num();
}

ARTSDKForceStateServerClientLockstep::ARTSDKForceStateServerClientLockstep(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

FText ARTSDKForceStateServerClientLockstep::GetDisplayName()
{
    return DisplayName;
}

void ARTSDKForceStateServerClientLockstep::SetDisplayName(const FText& inName)
{
    DisplayName = inName;
}

ARTSDKTeamStateBase* ARTSDKForceStateServerClientLockstep::GetTeam()
{
    return Team.Get();
}

void ARTSDKForceStateServerClientLockstep::SetTeam(ARTSDKTeamStateBase* inTeam)
{
    Team = inTeam;
}

TArray<ARTSDKCommanderStateBase*> ARTSDKForceStateServerClientLockstep::GetCommanders()
{
    return Commanders;
}

ARTSDKCommanderStateBase* ARTSDKForceStateServerClientLockstep::GetCommanderByIndex(int32 CommanderIndex)
{
    if (CommanderIndex < Commanders.Num())
    {
        return Commanders[CommanderIndex];
    }
    return nullptr;
}

void ARTSDKForceStateServerClientLockstep::SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders)
{
    Commanders = inCommanders;
}

int32 ARTSDKForceStateServerClientLockstep::AddCommander(ARTSDKCommanderStateBase* inCommander)
{
    return Commanders.Add(inCommander);
}

int32 ARTSDKForceStateServerClientLockstep::GetCommanderCount()
{
    return Commanders.Num();
}

void ARTSDKForceStateServerClientLockstep::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSDKForceStateServerClientLockstep, Team);
    DOREPLIFETIME(ARTSDKForceStateServerClientLockstep, Commanders);
    DOREPLIFETIME(ARTSDKForceStateServerClientLockstep, DisplayName);
}

ARTSDKForceStateServerClientCurves::ARTSDKForceStateServerClientCurves(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

FText ARTSDKForceStateServerClientCurves::GetDisplayName()
{
    return DisplayName;
}

void ARTSDKForceStateServerClientCurves::SetDisplayName(const FText& inName)
{
    DisplayName = inName;
}

ARTSDKTeamStateBase* ARTSDKForceStateServerClientCurves::GetTeam()
{
    return Team.Get();
}

void ARTSDKForceStateServerClientCurves::SetTeam(ARTSDKTeamStateBase* inTeam)
{
    Team = inTeam;
}

TArray<ARTSDKCommanderStateBase*> ARTSDKForceStateServerClientCurves::GetCommanders()
{
    return Commanders;
}

ARTSDKCommanderStateBase* ARTSDKForceStateServerClientCurves::GetCommanderByIndex(int32 CommanderIndex)
{
    if (CommanderIndex < Commanders.Num())
    {
        return Commanders[CommanderIndex];
    }
    return nullptr;
}

void ARTSDKForceStateServerClientCurves::SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders)
{
    Commanders = inCommanders;
}

int32 ARTSDKForceStateServerClientCurves::AddCommander(ARTSDKCommanderStateBase* inCommander)
{
    return Commanders.Add(inCommander);
}

int32 ARTSDKForceStateServerClientCurves::GetCommanderCount()
{
    return Commanders.Num();
}

void ARTSDKForceStateServerClientCurves::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSDKForceStateServerClientCurves, Team);
    DOREPLIFETIME(ARTSDKForceStateServerClientCurves, Commanders);
    DOREPLIFETIME(ARTSDKForceStateServerClientCurves, DisplayName);
}
