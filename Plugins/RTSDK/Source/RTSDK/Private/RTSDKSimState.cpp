// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKSimState.h"
#include "RTSDKConstants.h"
#include "FixedPointTypes.h"
#include "RTSDKCommanderState.h"
#include "RTSDKTeamState.h"
#include "RTSDKForceState.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKPlayerControllerInterface.h"
#include "GameFramework/PlayerState.h"
#include "RTSDKWorldSettings.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameSession.h"
#include "RTSDKDeveloperSettings.h"

ARTSDKSimStateBase::ARTSDKSimStateBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARTSDKSimStateBase::PlayerSetup()
{
    APlayerController* epicpc = GetWorld()->GetFirstPlayerController();

    if (epicpc == nullptr)
    {
        return;
    }
    if (!epicpc->Implements<URTSDKPlayerControllerInterface>())
    {
        return;
    }
    LocalPlayerController = epicpc;
}

bool ARTSDKSimStateBase::GetIsPlayerSetup()
{
    //there are no local players on dedicated servers to set up.
    if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
    {
        return true;
    }
    if (LocalPlayerController == nullptr)
    {
        return false;
    }
    return true;
}

void ARTSDKSimStateBase::Setup(URTSDKGameSimSubsystem* inSimSubsystem, UWorld* inWorld)
{
    SimSubsystem = inSimSubsystem;
    OptionsString = inWorld->GetAuthGameMode()->OptionsString;
#if WITH_EDITORONLY_DATA
    //PIE world settings based match setup
    //options string from url is ignored, rtsdk world settings 'PIEMatchSetup' property used instead
    if (inWorld->WorldType == EWorldType::PIE)
    {
        ARTSDKWorldSettings* ws = Cast<ARTSDKWorldSettings>(inWorld->GetWorldSettings());
        if (ws != nullptr)
        {
            OptionsString = FRTSDKLaunchOptionsHelpers::GetLaunchOptionsFromPIEMatchSetup(ws->PIEMatchSetup, ws->PIEMatchIsLAN);
        }
    }
#endif
    //pull each team, force and commander declaration out of options

    TMap<int32, FRTSDKStateSetupInfo> TeamOptionsMap;
    TMap<int32, FRTSDKStateSetupInfo> ForceOptionsMap;
    TMap<int32, FRTSDKStateSetupInfo> CommanderOptionsMap;
    TMap<FString, FString> OtherOptionsMap;
    if (FRTSDKLaunchOptionsHelpers::ExtractOptions(OptionsString, OtherOptionsMap, TeamOptionsMap, ForceOptionsMap, CommanderOptionsMap))
    {
        OptionsMap = OtherOptionsMap;
        SetupTeams(TeamOptionsMap);
        SetupForces(ForceOptionsMap);
        SetupCommanders(CommanderOptionsMap);
    }
}

void ARTSDKSimStateBase::BeginPlay()
{
    Super::BeginPlay();
    //set sim state in the subsystem locally for clients and begin running
    if (GetLocalRole() != ENetRole::ROLE_Authority)
    {
        URTSDKGameSimSubsystem* sim = GetWorld()->GetSubsystem<URTSDKGameSimSubsystem>();
        sim->SetSimState(this);
        sim->SetSimIsInitialized(true);
        SimSubsystem = sim;
    }
}

ARTSDKSimStateSPOnly::ARTSDKSimStateSPOnly(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = false;
}

void ARTSDKSimStateSPOnly::SetupTeams(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKTeamStateBase*> teams;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKTeamStateBase* team = world->SpawnActor<ARTSDKTeamStateBase>(RTSDKSettings->TeamStateClass);
        team->Setup(It->Value, this, SimSubsystem);
        teams.Add(team);
    }
    SetTeams(teams);
}

void ARTSDKSimStateSPOnly::SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKForceStateBase*> forces;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKForceStateBase* force = world->SpawnActor<ARTSDKForceStateBase>(RTSDKSettings->ForceStateClass);
        force->Setup(It->Value, this, SimSubsystem);
        forces.Add(force);
    }
    SetForces(forces);
}

void ARTSDKSimStateSPOnly::SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKCommanderStateBase*> commanders;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKCommanderStateBase* commander = world->SpawnActor<ARTSDKCommanderStateBase>(RTSDKSettings->CommanderStateClass);
        commander->Setup(It->Value, this, SimSubsystem);
        commanders.Add(commander);
    }
    SetCommanders(commanders);
}

void ARTSDKSimStateSPOnly::PreMatchTick()
{
    IRTSDKPlayerControllerInterface* localpc = Cast<IRTSDKPlayerControllerInterface>(LocalPlayerController);
    
    if (localpc->GetWantsToBeReady())
    {
        SetMatchHasStarted(true);
    }
}

void ARTSDKSimStateSPOnly::SetMatchHasStarted(bool inMatchHasStarted)
{
    bMatchHasStarted = inMatchHasStarted;
}

bool ARTSDKSimStateSPOnly::GetMatchHasStarted()
{
    return bMatchHasStarted;
}

void ARTSDKSimStateSPOnly::SetMatchIsPaused(bool inMatchIsPaused)
{
    bMatchIsPaused = inMatchIsPaused;
}

bool ARTSDKSimStateSPOnly::GetMatchIsPaused()
{
    return bMatchIsPaused;
}

void ARTSDKSimStateSPOnly::RequestPause(AController* inController)
{
    //just pause and unpause when the player asks for it
    SetMatchIsPaused(true);
}

void ARTSDKSimStateSPOnly::RequestUnpause(AController* inController)
{
    //just pause and unpause when the player asks for it
    SetMatchIsPaused(false);
}

void ARTSDKSimStateSPOnly::RequestTimescale(AController* inController, const FFixed64& inTimescale)
{
#if RTSDK_USE_FIXED_POINT
    SimSubsystem->SetTimeScale(inTimescale);
#else
    SimSubsystem->SetTimeScale((double)inTimescale);
#endif    
}

ERTSDKShouldAdvanceInputTurnResult ARTSDKSimStateSPOnly::ShouldAdvanceInputTurn()
{
    //turns happen every frame
    return ERTSDKShouldAdvanceInputTurnResult::Advance;
}

void ARTSDKSimStateSPOnly::OnPreAdvanceInputTurn()
{
    for (int32 i = 0; i < Commanders.Num(); i++)
    {
        Commanders[i]->FlushCommandBuffer();
    }
}

ARTSDKCommanderStateBase* ARTSDKSimStateSPOnly::GetCommander(const int32& inCommanderID)
{
    return inCommanderID < Commanders.Num() ? Commanders[inCommanderID] : nullptr;
}

TArray<ARTSDKCommanderStateBase*> ARTSDKSimStateSPOnly::GetCommanders()
{
    return Commanders;
}

int32 ARTSDKSimStateSPOnly::AddCommander(ARTSDKCommanderStateBase* inCommanderState)
{
    return Commanders.Add(inCommanderState);
}

void ARTSDKSimStateSPOnly::SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates)
{
    Commanders = inCommanderStates;
}

int32 ARTSDKSimStateSPOnly::GetCommanderCount()
{
    return Commanders.Num();
}

ARTSDKForceStateBase* ARTSDKSimStateSPOnly::GetForce(const int32& inForceID)
{
    if (inForceID < Forces.Num())
    {
        return Forces[inForceID];
    }
    return nullptr;
}

TArray<ARTSDKForceStateBase*> ARTSDKSimStateSPOnly::GetForces()
{
    return Forces;
}

int32 ARTSDKSimStateSPOnly::AddForce(ARTSDKForceStateBase* inForceState)
{
    return Forces.Add(inForceState);
}

void ARTSDKSimStateSPOnly::SetForces(TArray<ARTSDKForceStateBase*> inForceStates)
{
    Forces = inForceStates;
}

int32 ARTSDKSimStateSPOnly::GetForceCount()
{
    return Forces.Num();
}

ARTSDKTeamStateBase* ARTSDKSimStateSPOnly::GetTeam(const int32& inTeamID)
{
    if (inTeamID < Teams.Num())
    {
        return Teams[inTeamID];
    }
    return nullptr;
}

TArray<ARTSDKTeamStateBase*> ARTSDKSimStateSPOnly::GetTeams()
{
    return Teams;
}

int32 ARTSDKSimStateSPOnly::AddTeam(ARTSDKTeamStateBase* inTeamState)
{
    return Teams.Add(inTeamState);
}

void ARTSDKSimStateSPOnly::SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates)
{
    Teams = inTeamStates;
}

int32 ARTSDKSimStateSPOnly::GetTeamCount()
{
    return Teams.Num();
}

TArray<FRTSDKPlayerCommandReplicationInfo> ARTSDKSimStateSPOnly::GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn)
{
    if ((inCommanderID < Commanders.Num()) && (Commanders[inCommanderID] != nullptr))
    {
        return Commanders[inCommanderID]->GetCommandsByTurn(inTurn);
    }
    return TArray<FRTSDKPlayerCommandReplicationInfo>();
}

ARTSDKSimStateServerClientLockstep::ARTSDKSimStateServerClientLockstep(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    bAlwaysRelevant = true;
    FramesPerTurn = -1;
    TurnDuration = -1.0;
}

bool ARTSDKSimStateServerClientLockstep::GetIsPlayerSetup()
{
    bool superresult = Super::GetIsPlayerSetup();
    if (!superresult)
    {
        return false;
    }
    //clients need to wait for things to replicate over, this is particularly true of PIE matches, which auto ready-up players
    if (GetLocalRole() != ENetRole::ROLE_Authority)
    {
        for (int32 i = 0; i < Teams.Num(); i++)
        {
            if (Teams[i] == nullptr)
            {
                return false;
            }
        }
        for (int32 i = 0; i < Forces.Num(); i++)
        {
            if (Forces[i] == nullptr)
            {
                return false;
            }
        }
        for (int32 i = 0; i < Commanders.Num(); i++)
        {
            if (Commanders[i] == nullptr)
            {
                return false;
            }
        }
    }
    return true;
}

void ARTSDKSimStateServerClientLockstep::Setup(URTSDKGameSimSubsystem* inSimSubsystem, UWorld* inWorld)
{
    Super::Setup(inSimSubsystem, inWorld);
    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    if (inWorld->GetNetMode() == ENetMode::NM_Standalone)
    {
        MinTurnDuration = 0.0;
    }
    else if (OptionsMap.Contains(TEXT("bIsLANMatch")))
    {
        MinTurnDuration = RTSDKSettings->MinimumLANTurnDuration;
    }
    else
    {
        MinTurnDuration = RTSDKSettings->MinimumNetTurnDuration;
    }
    InitialTurnDuration = -1.0;
    LockstepTimeoutTurnCount = RTSDKSettings->LockstepTimeoutTurnCount;
    FramesPerTurn = -1;
}

void ARTSDKSimStateServerClientLockstep::SetupTeams(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKTeamStateBase*> teams;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKTeamStateBase* team = world->SpawnActor<ARTSDKTeamStateBase>(RTSDKSettings->TeamStateClass);
        team->Setup(It->Value, this, SimSubsystem);
        teams.Add(team);
    }
    SetTeams(teams);
}

void ARTSDKSimStateServerClientLockstep::SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKForceStateBase*> forces;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKForceStateBase* force = world->SpawnActor<ARTSDKForceStateBase>(RTSDKSettings->ForceStateClass);
        force->Setup(It->Value, this, SimSubsystem);
        forces.Add(force);
    }
    SetForces(forces);
}

void ARTSDKSimStateServerClientLockstep::SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKCommanderStateBase*> commanders;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKCommanderStateBase* commander = world->SpawnActor<ARTSDKCommanderStateBase>(RTSDKSettings->CommanderStateClass);
        commander->Setup(It->Value, this, SimSubsystem);
        commanders.Add(commander);
    }
    SetCommanders(commanders);
}

void ARTSDKSimStateServerClientLockstep::PreMatchTick()
{
    UWorld* world = GetWorld();
    ENetMode netmode = world->GetNetMode();
#if WITH_EDITORONLY_DATA
    if ((world->WorldType == EWorldType::PIE) && (netmode != ENetMode::NM_Client))
    {
        int32 i = 0;
        for (FConstPlayerControllerIterator Iterator = world->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            APlayerController* PlayerController = Iterator->Get();
            if (PlayerController->PlayerState != nullptr)
            {
                PlayerController->PlayerState->SetPlayerId(i);
                i++;
            }
        }
        int32 playercommanders = 0;
        for (int32 c = 0; c < Commanders.Num(); c++)
        {
            playercommanders = Commanders[c]->GetIsPlayer() ? playercommanders + 1 : playercommanders;
        }
        if (i < playercommanders)
        {
            //players aren't all in yet
            return;
        }
    }
#endif
    //if some sort of server, other than standalone, which is singleplayer, we try to set all commanders for players
    if ((netmode == ENetMode::NM_DedicatedServer) || (netmode == ENetMode::NM_ListenServer))
    {
        for (FConstPlayerControllerIterator Iterator = world->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            APlayerController* PlayerController = Iterator->Get();
            IRTSDKPlayerControllerInterface* pc = Cast<IRTSDKPlayerControllerInterface>(PlayerController);
            if (pc == nullptr)
            {
                continue;
            }
            //ignore anyone that has theirs set already.
            if (pc->GetCommanderState() != nullptr)
            {
                continue;
            }

            for (int32 i = 0; i < Commanders.Num(); i++)
            {
                if (!Commanders[i]->GetIsPlayer())
                {
                    continue;
                }
                if (Commanders[i]->GetPlayerID() == PlayerController->GetPlayerState<APlayerState>()->GetPlayerId())
                {
                    pc->SetCommanderState(Commanders[i]);
                }
            }
        }
    }
    
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        //ready check
        for (int32 i = 0; i < Commanders.Num(); i++)
        {
            if (!Commanders[i]->GetIsReady())
            {
                return;
            }
        }

        InitialTurnDuration = CalculateTurnDuration();
        TurnDuration = InitialTurnDuration;
        FramesPerTurn = CalculateFramesPerTurn();
        SetMatchHasStarted(true);
    }
    else
    {
        //if we have a valid initial turn duration and turn duration is not valid
        if ((InitialTurnDuration >= FFixed64::MakeFromRawInt(0)) && (TurnDuration < FFixed64::MakeFromRawInt(0)))
        {
            TurnDuration = InitialTurnDuration;
            FramesPerTurn = CalculateFramesPerTurn();
        }
    }
}

void ARTSDKSimStateServerClientLockstep::SetMatchHasStarted(bool inMatchHasStarted)
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        bMatchHasStarted = inMatchHasStarted;
    }
}

bool ARTSDKSimStateServerClientLockstep::GetMatchHasStarted()
{
    if (InitialTurnDuration < FFixed64::MakeFromRawInt(0))
    {
        return false;
    }
    if (TurnDuration < FFixed64::MakeFromRawInt(0))
    {
        return false;
    }
    if (FramesPerTurn < 0)
    {
        return false;
    }
    return bMatchHasStarted;
}

void ARTSDKSimStateServerClientLockstep::SetMatchIsPaused(bool inMatchIsPaused)
{
    bMatchIsPaused = inMatchIsPaused;
}

bool ARTSDKSimStateServerClientLockstep::GetMatchIsPaused()
{
    return bMatchIsPaused;
}

void ARTSDKSimStateServerClientLockstep::RequestPause(AController* inController)
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        ARTSDKCommanderStateBase* cmdr = nullptr;
        if (PlayerMayRequestPause(inController, cmdr))
        {
            if (!Pausers.Contains(cmdr))
            {
                Pausers.Add(cmdr);
            }
        }
    }
}

void ARTSDKSimStateServerClientLockstep::RequestUnpause(AController* inController)
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        ARTSDKCommanderStateBase* cmdr = nullptr;
        if (PlayerMayRequestUnpause(inController, cmdr))
        {
            Pausers.Remove(cmdr);
            if (Pausers.Num() == 0)
            {
                //unpause everyone
                Multicast_OnUnpauseMatch();
            }
        }
    }
}

void ARTSDKSimStateServerClientLockstep::RequestTimescale(AController* inController, const FFixed64& inTimescale)
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        if (PlayerMayRequestTimescale(inController, inTimescale))
        {
            DesiredTimescale = inTimescale;
        }
    }
}

ERTSDKShouldAdvanceInputTurnResult ARTSDKSimStateServerClientLockstep::ShouldAdvanceInputTurn()
{
    if (GetMatchIsPaused())
    {
        return ERTSDKShouldAdvanceInputTurnResult::Wait;
    }
    int32 framecount = SimSubsystem->GetFrameCount();
    UWorld* world = GetWorld();
    //if we are in singleplayer advance every frame.
    //todo: replay playback check here for not advancing because of standalone
    if (world->GetNetMode() == ENetMode::NM_Standalone)
    {
        return ERTSDKShouldAdvanceInputTurnResult::Advance;
    }
    if ((LastTurnFrame + FramesPerTurn) <= framecount)
    {
        int32 currentturn = SimSubsystem->GetCurrentInputTurn();
        if (world->GetNetMode() == ENetMode::NM_Client)
        {
            if (!PauseCommands.HasTurn(currentturn))
            {
                return ERTSDKShouldAdvanceInputTurnResult::Wait;
            }
            if (!TimescaleCommands.HasTurn(currentturn))
            {
                return ERTSDKShouldAdvanceInputTurnResult::Wait;
            }
            if (!TurnDurationCommands.HasTurn(currentturn))
            {
                return ERTSDKShouldAdvanceInputTurnResult::Wait;
            }
            for (int32 i = 0; i < Commanders.Num(); i++)
            {
                if (!Commanders[i]->HasTurn(currentturn))
                {
                    return ERTSDKShouldAdvanceInputTurnResult::Wait;
                }
            }
        }
        else
        {
            //check if any participating players have fallen behind. 
            for (int32 i = 0; i < Commanders.Num(); i++)
            {
                if (!Commanders[i]->GetIsPlayer())
                {
                    continue;
                }
                if (currentturn - Commanders[i]->GetLastCompletedTurn() >= LockstepTimeoutTurnCount)
                {
                    return ERTSDKShouldAdvanceInputTurnResult::Wait;
                }
            }
        }
        LastTurnFrame = framecount;
        return ERTSDKShouldAdvanceInputTurnResult::Advance;
    }
    //we are within a turn and unpaused, advance frame, we aren't ready to advance turn.
    return ERTSDKShouldAdvanceInputTurnResult::Skip;
}

void ARTSDKSimStateServerClientLockstep::OnPreAdvanceInputTurn()
{
    ENetMode netmode = GetWorld()->GetNetMode();
    if (netmode != ENetMode::NM_Client)
    {
        for (int32 i = 0; i < Commanders.Num(); i++)
        {
            Commanders[i]->FlushCommandBuffer();
        }
    }
    int32 currentturn = SimSubsystem->GetCurrentInputTurn();
    //if we have a pause command we execute that now.
    //Turn advancement will return wait until unpaused.
   /* if (PauseCommands.HasPauseCommandOnTurn(currentturn))
    {
        SetMatchIsPaused(true);
    }*/

    AdvancePauseCommands();
    AdvanceTimescaleCommands();
    AdvanceTurnDurationCommands();

    if (netmode == ENetMode::NM_DedicatedServer)
    {
        return;//no local player controller.
    }
    //todo, generate checksum for desync checks
    Cast<IRTSDKPlayerControllerInterface>(LocalPlayerController)->FinishInputTurn(currentturn, 0);
}

ARTSDKCommanderStateBase* ARTSDKSimStateServerClientLockstep::GetCommander(const int32& inCommanderID)
{
    if (inCommanderID < Commanders.Num())
    {
        return Commanders[inCommanderID];
    }
    return nullptr;
}

TArray<ARTSDKCommanderStateBase*> ARTSDKSimStateServerClientLockstep::GetCommanders()
{
    return Commanders;
}

int32 ARTSDKSimStateServerClientLockstep::AddCommander(ARTSDKCommanderStateBase* inCommanderState)
{
    return Commanders.Add(inCommanderState);
}

void ARTSDKSimStateServerClientLockstep::SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates)
{
    Commanders = inCommanderStates;
}

int32 ARTSDKSimStateServerClientLockstep::GetCommanderCount()
{
    return Commanders.Num();
}

ARTSDKForceStateBase* ARTSDKSimStateServerClientLockstep::GetForce(const int32& inForceID)
{
    if (inForceID < Forces.Num())
    {
        return Forces[inForceID];
    }
    return nullptr;
}

TArray<ARTSDKForceStateBase*> ARTSDKSimStateServerClientLockstep::GetForces()
{
    return Forces;
}

int32 ARTSDKSimStateServerClientLockstep::AddForce(ARTSDKForceStateBase* inForceState)
{
    return Forces.Add(inForceState);
}

void ARTSDKSimStateServerClientLockstep::SetForces(TArray<ARTSDKForceStateBase*> inForceStates)
{
    Forces = inForceStates;
}

int32 ARTSDKSimStateServerClientLockstep::GetForceCount()
{
    return Forces.Num();
}

ARTSDKTeamStateBase* ARTSDKSimStateServerClientLockstep::GetTeam(const int32& inTeamID)
{
    if (inTeamID < Teams.Num())
    {
        return Teams[inTeamID];
    }
    return nullptr;
}

TArray<ARTSDKTeamStateBase*> ARTSDKSimStateServerClientLockstep::GetTeams()
{
    return Teams;
}

int32 ARTSDKSimStateServerClientLockstep::AddTeam(ARTSDKTeamStateBase* inTeamState)
{
    return Teams.Add(inTeamState);
}

void ARTSDKSimStateServerClientLockstep::SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates)
{
    Teams = inTeamStates;
}

int32 ARTSDKSimStateServerClientLockstep::GetTeamCount()
{
    return Teams.Num();
}

TArray<FRTSDKPlayerCommandReplicationInfo> ARTSDKSimStateServerClientLockstep::GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn)
{
    if ((inCommanderID < Commanders.Num()) && (Commanders[inCommanderID] != nullptr))
    {
        return Commanders[inCommanderID]->GetCommandsByTurn(inTurn);
    }
    return TArray<FRTSDKPlayerCommandReplicationInfo>();
}

bool ARTSDKSimStateServerClientLockstep::PlayerMayRequestPause(AController* inController, ARTSDKCommanderStateBase*& outCommander)
{
    //default implementation allows free pausing and unpausing for participants, not spectators.
    IRTSDKPlayerControllerInterface* pc = Cast<IRTSDKPlayerControllerInterface>(inController);
    if (pc != nullptr)
    {
        ARTSDKCommanderStateBase* cmdr = pc->GetCommanderState();
        if (cmdr != nullptr)
        {
            outCommander = cmdr;
            return true;
        }
    }
    return false;
}

bool ARTSDKSimStateServerClientLockstep::PlayerMayRequestUnpause(AController* inController, ARTSDKCommanderStateBase*& outCommander)
{
    //we can't unpause a match that isn't paused.
    if (!GetMatchIsPaused())
    {
        return false;
    }
    //default implementation allows free pausing and unpausing for participants, not spectators.
    IRTSDKPlayerControllerInterface* pc = Cast<IRTSDKPlayerControllerInterface>(inController);
    if (pc != nullptr)
    {
        ARTSDKCommanderStateBase* cmdr = pc->GetCommanderState();
        if (cmdr != nullptr)
        {
            //only if they are an existing pauser
            if (Pausers.Contains(cmdr))
            {
                outCommander = cmdr;
                return true;
            }
        }
    }
    return false;
}

bool ARTSDKSimStateServerClientLockstep::PlayerMayRequestTimescale(AController* inController, const FFixed64& inTimescale)
{
    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    if ((inTimescale >= FFixed64::MakeFromRawInt(0)) && (inTimescale <= RTSDKSettings->MaxTimescale))
    {
        //default implementation allows free timescale changes for participants, not spectators, if the timescale is within accepted range (0-max)
        IRTSDKPlayerControllerInterface* pc = Cast<IRTSDKPlayerControllerInterface>(inController);
        if (pc != nullptr)
        {
            ARTSDKCommanderStateBase* cmdr = pc->GetCommanderState();
            if (cmdr != nullptr)
            {
                return true;
            }
        }
    }
    return false;
}

void ARTSDKSimStateServerClientLockstep::Multicast_OnUnpauseMatch_Implementation()
{
    SetMatchIsPaused(false);
}

void ARTSDKSimStateServerClientLockstep::AdvancePauseCommands()
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        if (Pausers.Num() > 0)
        {
            PauseCommands.AddPauseCommand(SimSubsystem->GetCurrentInputTurn(), true);
            SetMatchIsPaused(true);
        }
        else
        {
            PauseCommands.AddPauseCommand(SimSubsystem->GetCurrentInputTurn(), false);
        }
    }
    else
    {
        if (PauseCommands.HasPauseCommandOnTurn(SimSubsystem->GetCurrentInputTurn()))
        {
            SetMatchIsPaused(true);
        }
    }
}

void ARTSDKSimStateServerClientLockstep::AdvanceTimescaleCommands()
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        if (DesiredTimescale > FFixed64::MakeFromRawInt(0))
        {
            TimescaleCommands.AddTimescaleCommand(SimSubsystem->GetCurrentInputTurn(), DesiredTimescale);
#if RTSDK_USE_FIXED_POINT
            SimSubsystem->SetTimeScale(DesiredTimescale);
#else
            SimSubsystem->SetTimeScale((double)DesiredTimescale);
#endif

            DesiredTimescale = -1.0;
            FramesPerTurn = CalculateFramesPerTurn();
        }
        else
        {
            TimescaleCommands.AddEmptyTimescaleCommand(SimSubsystem->GetCurrentInputTurn());
        }
    }
    else
    {
        FFixed64 timescale;
        if (TimescaleCommands.HasTimescaleCommandOnTurn(SimSubsystem->GetCurrentInputTurn(), timescale))
        {
#if RTSDK_USE_FIXED_POINT
            SimSubsystem->SetTimeScale(timescale);
#else
            SimSubsystem->SetTimeScale((double)timescale);
#endif
            FramesPerTurn = CalculateFramesPerTurn();
        }
    }
}

void ARTSDKSimStateServerClientLockstep::AdvanceTurnDurationCommands()
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
        FFixed64 newoptimalduration = CalculateTurnDuration();

        if (FFixedPointMath::Abs(newoptimalduration - TurnDuration) > RTSDKSettings->LockstepTurnDurationTolerance)
        {
            TurnDurationCommands.AddTurnDurationCommand(SimSubsystem->GetCurrentInputTurn(), newoptimalduration);
            TurnDuration = newoptimalduration;

            FramesPerTurn = CalculateFramesPerTurn();
        }
        else
        {
            TurnDurationCommands.AddEmptyTurnDurationCommand(SimSubsystem->GetCurrentInputTurn());
        }
    }
    else
    {
        FFixed64 turnduration;
        if (TurnDurationCommands.HasTurnDurationCommandOnTurn(SimSubsystem->GetCurrentInputTurn(), turnduration))
        {
            TurnDuration = turnduration;
            FramesPerTurn = CalculateFramesPerTurn();
        }
    }
}

int32 ARTSDKSimStateServerClientLockstep::CalculateFramesPerTurn()
{
    //todo: interop module for deterministic maths, this is starting to get annoying!
    //FFixed64 durationoverscaledtimestep = TurnDuration / (FFixed64((double)SimSubsystem->GetTimestep()) * FFixed64((double)SimSubsystem->GetTimeScale()));
    FFixed64 scaleddurationovertimestep = (TurnDuration * FFixed64((double)SimSubsystem->GetTimeScale())) / FFixed64((double)SimSubsystem->GetTimestep());
    return FFixedPointMath::CeilToInt(scaleddurationovertimestep);
}

FFixed64 ARTSDKSimStateServerClientLockstep::CalculateTurnDuration()
{
    UWorld* world = GetWorld();
    float highestping = 0.0f;
    for (FConstPlayerControllerIterator Iterator = world->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PlayerController = Iterator->Get();
        if (PlayerController->PlayerState == nullptr)
        {
            continue;
        }
        IRTSDKPlayerControllerInterface* pc = Cast<IRTSDKPlayerControllerInterface>(PlayerController);
        if (pc == nullptr)
        {
            continue;
        }
        //ignore anyone that isn't a participating player
        if (pc->GetCommanderState() == nullptr)
        {
            continue;
        }
        float ping = PlayerController->PlayerState->GetPingInMilliseconds();
        if (ping > highestping)
        {
            highestping = ping;
        }
    }
    return FFixedPointMath::Max(MinTurnDuration, FFixed64(highestping / 1000.0));
}

void ARTSDKSimStateServerClientLockstep::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, InitialTurnDuration);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, Commanders);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, Teams);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, Forces);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, bMatchHasStarted);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, PauseCommands);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, Pausers);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, TimescaleCommands);
    DOREPLIFETIME(ARTSDKSimStateServerClientLockstep, TurnDurationCommands);
}

ARTSDKSimStateServerClientCurves::ARTSDKSimStateServerClientCurves(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    bAlwaysRelevant = true;
}

void ARTSDKSimStateServerClientCurves::SetupTeams(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKTeamStateBase*> teams;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKTeamStateBase* team = world->SpawnActor<ARTSDKTeamStateBase>(RTSDKSettings->TeamStateClass);
        team->Setup(It->Value, this, SimSubsystem);
        teams.Add(team);
    }
    SetTeams(teams);
}

void ARTSDKSimStateServerClientCurves::SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKForceStateBase*> forces;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKForceStateBase* force = world->SpawnActor<ARTSDKForceStateBase>(RTSDKSettings->ForceStateClass);
        force->Setup(It->Value, this, SimSubsystem);
        forces.Add(force);
    }
    SetForces(forces);
}

void ARTSDKSimStateServerClientCurves::SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap)
{
    UWorld* world = GetWorld();
    TArray<ARTSDKCommanderStateBase*> commanders;

    const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
    for (auto It = inOptionsMap.CreateConstIterator(); It; ++It)
    {
        ARTSDKCommanderStateBase* commander = world->SpawnActor<ARTSDKCommanderStateBase>(RTSDKSettings->CommanderStateClass);
        commander->Setup(It->Value, this, SimSubsystem);
        commanders.Add(commander);
    }
    SetCommanders(commanders);
}

void ARTSDKSimStateServerClientCurves::SetMatchHasStarted(bool inMatchHasStarted)
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        bMatchHasStarted = inMatchHasStarted;
    }
}

bool ARTSDKSimStateServerClientCurves::GetMatchHasStarted()
{
    return bMatchHasStarted;
}

void ARTSDKSimStateServerClientCurves::SetMatchIsPaused(bool inMatchIsPaused)
{
    if (GetLocalRole() == ENetRole::ROLE_Authority)
    {
        bMatchIsPaused = inMatchIsPaused;
    }
}

bool ARTSDKSimStateServerClientCurves::GetMatchIsPaused()
{
    return bMatchIsPaused;
}

void ARTSDKSimStateServerClientCurves::RequestPause(AController* inController)
{
}

void ARTSDKSimStateServerClientCurves::RequestUnpause(AController* inController)
{
}

ERTSDKShouldAdvanceInputTurnResult ARTSDKSimStateServerClientCurves::ShouldAdvanceInputTurn()
{
    //we just advance turn every frame because only server runs commands.
    return ERTSDKShouldAdvanceInputTurnResult::Advance;
}

void ARTSDKSimStateServerClientCurves::OnPreAdvanceInputTurn()
{
    if (GetWorld()->GetNetMode() != NM_Client)
    {
        for (int32 i = 0; i < Commanders.Num(); i++)
        {
            Commanders[i]->FlushCommandBuffer();
        }
    }
}

ARTSDKCommanderStateBase* ARTSDKSimStateServerClientCurves::GetCommander(const int32& inCommanderID)
{
    if (inCommanderID < Commanders.Num())
    {
        return Commanders[inCommanderID];
    }
    return nullptr;
}

TArray<ARTSDKCommanderStateBase*> ARTSDKSimStateServerClientCurves::GetCommanders()
{
    return Commanders;
}

int32 ARTSDKSimStateServerClientCurves::AddCommander(ARTSDKCommanderStateBase* inCommanderState)
{
    return Commanders.Add(inCommanderState);
}

void ARTSDKSimStateServerClientCurves::SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates)
{
    Commanders = inCommanderStates;
}

int32 ARTSDKSimStateServerClientCurves::GetCommanderCount()
{
    return Commanders.Num();
}

ARTSDKForceStateBase* ARTSDKSimStateServerClientCurves::GetForce(const int32& inForceID)
{
    if (inForceID < Forces.Num())
    {
        return Forces[inForceID];
    }
    return nullptr;
}

TArray<ARTSDKForceStateBase*> ARTSDKSimStateServerClientCurves::GetForces()
{
    return Forces;
}

int32 ARTSDKSimStateServerClientCurves::AddForce(ARTSDKForceStateBase* inForceState)
{
    return Forces.Add(inForceState);
}

void ARTSDKSimStateServerClientCurves::SetForces(TArray<ARTSDKForceStateBase*> inForceStates)
{
    Forces = inForceStates;
}

int32 ARTSDKSimStateServerClientCurves::GetForceCount()
{
    return Forces.Num();
}

ARTSDKTeamStateBase* ARTSDKSimStateServerClientCurves::GetTeam(const int32& inTeamID)
{
    if (inTeamID < Teams.Num())
    {
        return Teams[inTeamID];
    }
    return nullptr;
}

TArray<ARTSDKTeamStateBase*> ARTSDKSimStateServerClientCurves::GetTeams()
{
    return Teams;
}

int32 ARTSDKSimStateServerClientCurves::AddTeam(ARTSDKTeamStateBase* inTeamState)
{
    return Teams.Add(inTeamState);
}

void ARTSDKSimStateServerClientCurves::SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates)
{
    Teams = inTeamStates;
}

int32 ARTSDKSimStateServerClientCurves::GetTeamCount()
{
    return Teams.Num();
}

TArray<FRTSDKPlayerCommandReplicationInfo> ARTSDKSimStateServerClientCurves::GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn)
{
    if ((inCommanderID < Commanders.Num()) && (Commanders[inCommanderID] != nullptr))
    {
        return Commanders[inCommanderID]->GetCommandsByTurn(inTurn);
    }
    return TArray<FRTSDKPlayerCommandReplicationInfo>();
}

void ARTSDKSimStateServerClientCurves::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARTSDKSimStateServerClientCurves, Commanders);
    DOREPLIFETIME(ARTSDKSimStateServerClientCurves, Teams);
    DOREPLIFETIME(ARTSDKSimStateServerClientCurves, Forces);
    DOREPLIFETIME(ARTSDKSimStateServerClientCurves, bMatchHasStarted);
    DOREPLIFETIME(ARTSDKSimStateServerClientCurves, bMatchIsPaused);
    DOREPLIFETIME(ARTSDKSimStateServerClientCurves, Pausers);
}

void FRTSDKLockstepPauseCommand::PreReplicatedRemove(const FRTSDKLockstepPauseCommands& InArraySerializer)
{
}

void FRTSDKLockstepPauseCommand::PostReplicatedAdd(const FRTSDKLockstepPauseCommands& InArraySerializer)
{
}

void FRTSDKLockstepPauseCommand::PostReplicatedChange(const FRTSDKLockstepPauseCommands& InArraySerializer)
{
}

void FRTSDKLockstepPauseCommands::AddPauseCommand(int32 inTurn, bool inDoPause)
{
    FRTSDKLockstepPauseCommand cmd;
    cmd.Turn = inTurn;
    cmd.bDoPause = inDoPause;
    int32 Idx = PauseCommands.Add(cmd);
    MarkItemDirty(PauseCommands[Idx]);

    // server calls "on rep" also
    PauseCommands[Idx].PostReplicatedAdd(*this);
}

bool FRTSDKLockstepPauseCommands::HasPauseCommandOnTurn(int32 inTurn)
{
    for (int32 i = 0; i < PauseCommands.Num(); i++)
    {
        if ((PauseCommands[i].Turn == inTurn) && (PauseCommands[i].bDoPause))
        {
            return true;
        }
    }
    return false;
}

bool FRTSDKLockstepPauseCommands::HasTurn(int32 inTurn)
{
    for (int32 i = 0; i < PauseCommands.Num(); i++)
    {
        if (PauseCommands[i].Turn == inTurn)
        {
            return true;
        }
    }
    return false;
}

void FRTSDKLockstepTimescaleCommand::PreReplicatedRemove(const FRTSDKLockstepTimescaleCommands& InArraySerializer)
{
}

void FRTSDKLockstepTimescaleCommand::PostReplicatedAdd(const FRTSDKLockstepTimescaleCommands& InArraySerializer)
{
}

void FRTSDKLockstepTimescaleCommand::PostReplicatedChange(const FRTSDKLockstepTimescaleCommands& InArraySerializer)
{
}

void FRTSDKLockstepTimescaleCommands::AddTimescaleCommand(int32 inTurn, FFixed64 inTimescale)
{
    FRTSDKLockstepTimescaleCommand cmd;
    cmd.Turn = inTurn;
    cmd.Timescale = inTimescale;
    int32 Idx = TimescaleCommands.Add(cmd);
    MarkItemDirty(TimescaleCommands[Idx]);

    // server calls "on rep" also
    TimescaleCommands[Idx].PostReplicatedAdd(*this);
}

void FRTSDKLockstepTimescaleCommands::AddEmptyTimescaleCommand(int32 inTurn)
{
    AddTimescaleCommand(inTurn, -1.0);
}

bool FRTSDKLockstepTimescaleCommands::HasTimescaleCommandOnTurn(int32 inTurn, FFixed64& outTimescale)
{
    for (int32 i = 0; i < TimescaleCommands.Num(); i++)
    {
        if ((TimescaleCommands[i].Turn == inTurn) && (TimescaleCommands[i].Timescale >= FFixed64::MakeFromRawInt(0)))
        {
            outTimescale = TimescaleCommands[i].Timescale;
            return true;
        }
    }
    return false;
}

bool FRTSDKLockstepTimescaleCommands::HasTurn(int32 inTurn)
{
    for (int32 i = 0; i < TimescaleCommands.Num(); i++)
    {
        if (TimescaleCommands[i].Turn == inTurn)
        {
            return true;
        }
    }
    return false;
}

void FRTSDKLockstepTurnDurationCommand::PreReplicatedRemove(const FRTSDKLockstepTurnDurationCommands& InArraySerializer)
{
}

void FRTSDKLockstepTurnDurationCommand::PostReplicatedAdd(const FRTSDKLockstepTurnDurationCommands& InArraySerializer)
{
}

void FRTSDKLockstepTurnDurationCommand::PostReplicatedChange(const FRTSDKLockstepTurnDurationCommands& InArraySerializer)
{
}

void FRTSDKLockstepTurnDurationCommands::AddTurnDurationCommand(int32 inTurn, FFixed64 inTurnDuration)
{
    FRTSDKLockstepTurnDurationCommand cmd;
    cmd.Turn = inTurn;
    cmd.TurnDuration = inTurnDuration;
    int32 Idx = TurnDurationCommands.Add(cmd);
    MarkItemDirty(TurnDurationCommands[Idx]);

    // server calls "on rep" also
    TurnDurationCommands[Idx].PostReplicatedAdd(*this);
}

void FRTSDKLockstepTurnDurationCommands::AddEmptyTurnDurationCommand(int32 inTurn)
{
    AddTurnDurationCommand(inTurn, -1.0);
}

bool FRTSDKLockstepTurnDurationCommands::HasTurnDurationCommandOnTurn(int32 inTurn, FFixed64& outTurnDuration)
{
    for (int32 i = 0; i < TurnDurationCommands.Num(); i++)
    {
        if ((TurnDurationCommands[i].Turn == inTurn) && (TurnDurationCommands[i].TurnDuration >= FFixed64::MakeFromRawInt(0)))
        {
            outTurnDuration = TurnDurationCommands[i].TurnDuration;
            return true;
        }
    }
    return false;
}

bool FRTSDKLockstepTurnDurationCommands::HasTurn(int32 inTurn)
{
    for (int32 i = 0; i < TurnDurationCommands.Num(); i++)
    {
        if (TurnDurationCommands[i].Turn == inTurn)
        {
            return true;
        }
    }
    return false;
}
