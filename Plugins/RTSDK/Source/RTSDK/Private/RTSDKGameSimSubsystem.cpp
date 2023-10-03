// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameSimSubsystem.h"
#include "RTSDKMassModuleSettings.h"
#include "RTSDKDeveloperSettings.h"
#include "RTSDKLobbySubsystem.h"
#include "MassExecutor.h"
#include "MassEntitySubsystem.h"
#include "MassSignalSubsystem.h"
#include "RTSDKGameStateBase.h"
#include "RTSDKSimState.h"
#include "RTSDKTeamState.h"
#include "RTSDKForceState.h"
#include "RTSDKCommanderState.h"
#include "RTSDKWorldSettings.h"
#include "RTSDKUnitDefinition.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKUnitSpawner.h"
#include "MassSpawnerSubsystem.h"
#include "MassEntityView.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "RTSDKModManager.h"

void URTSDKGameSimSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	bInScriptCallingMode = false;
	ResetFrameCount();
	ResetUnits();
	//SetFrameDelay(1);
	
	Super::Initialize(Collection);
}

void URTSDKGameSimSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void URTSDKGameSimSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{

	MassSpawnerSubsystem = InWorld.GetSubsystem<UMassSpawnerSubsystem>();
	UMassEntitySubsystem* EntitySubsystem = InWorld.GetSubsystem<UMassEntitySubsystem>();
	EntityManager = EntitySubsystem->GetMutableEntityManager().AsShared();
	//RebuildProcessors();

	const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
	SetTimeScale(1.0);
	SetMaxFramesPerTick(RTSDKSettings->MaxFramesPerGameThreadTick);
	SetTargetUPS(RTSDKSettings->TargetSimFramesPerSecond);
	
	SetMetersToUUScale(InWorld.GetWorldSettings()->WorldToMeters);
	SetGravityDirection(FVector::DownVector);
	SetGravityAcceleration(9.8);
	SetTerminalVelocity(40.0);
	bLevelUnitsInitialized = false;
	bSimProcessorsInitialized = false;
	LobbySubsystem = InWorld.GetGameInstance()->GetSubsystem<URTSDKLobbySubsystem>();
#if WITH_EDITORONLY_DATA
	if (InWorld.WorldType == EWorldType::PIE)
	{
		ARTSDKWorldSettings* worldsettings = Cast<ARTSDKWorldSettings>(InWorld.GetWorldSettings());
		LobbySubsystem->CreateNewLobbyPIE(&InWorld, worldsettings->PIEGameModName, worldsettings->PIEFactionModNames, worldsettings->PIEMapModName, worldsettings->PIEMutatorNames);
	}
#endif
	LastRealTimeSeconds = InWorld.RealTimeSeconds;
	bSimIsPaused = false;
	if (InWorld.GetNetMode() != NM_Client)
	{
		ARTSDKSimStateBase* simstate = InWorld.SpawnActor<ARTSDKSimStateBase>(RTSDKSettings->SimStateClass);
		simstate->Setup(this, &InWorld);
		SimState = simstate;
		bSimIsInitialized = true;
	}
}

void URTSDKGameSimSubsystem::Deinitialize()
{
#if WITH_EDITORONLY_DATA
	UWorld* world = GetWorld();
	if (world->WorldType == EWorldType::PIE)
	{
		if (LobbySubsystem != nullptr)
		{
			LobbySubsystem->DestroyLobbyPIE(world);
		}
	}
#endif
	UnitsByID.Empty();
	bSimIsInitialized = false;
	bInScriptCallingMode = false;
	ScriptCommandBuffer.Reset();
	Super::Deinitialize();
}

TStatId URTSDKGameSimSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URTSDKGameSimSubsystem, STATGROUP_Tickables);
}

void URTSDKGameSimSubsystem::Tick(float DeltaTime)
{
	if (!bSimIsInitialized)
	{
		return;
	}

	if (!LobbySubsystem->IsFullyLoaded())
	{
		return;
	}

	if (!SimState->GetIsPlayerSetup())
	{
		SimState->PlayerSetup();
		return;
	}
	
	if (!SimState->GetMatchHasStarted())
	{
		SimState->PreMatchTick();
		return;
	}

	if (!bSimProcessorsInitialized)
	{
		RebuildProcessors();
		bSimProcessorsInitialized = true;
	}

	if (!bLevelUnitsInitialized)
	{
		UWorld* world = GetWorld();
		TArray<ARTSDKUnitSpawner*> spawners;
		for (TActorIterator<ARTSDKUnitSpawner> It(world, ARTSDKUnitSpawner::StaticClass()); It; ++It)
		{
			ARTSDKUnitSpawner* spawner = *It;
			spawners.Add(spawner);
			/*uint32 unitid;
			SpawnUnit(spawner->UnitType, unitid, spawner->ForceID, spawner->GetActorLocation(), spawner->GetActorRotation(), false);*/
		}
		spawners.Sort
		(
			[](const ARTSDKUnitSpawner& A, const ARTSDKUnitSpawner& B)
			{
				return A.GetPathName() < B.GetPathName();
			}
		);

		for (int32 i = 0; i < spawners.Num(); i++)
		{
			uint32 unitid;
			SpawnUnit(spawners[i]->UnitType, unitid, spawners[i]->ForceID, spawners[i]->GetActorLocation(), spawners[i]->GetActorRotation(), false);
		}
		bLevelUnitsInitialized = true;
	}

	FRTSNumber64 realtimeseconds = GetWorld()->RealTimeSeconds;
	FRTSNumber64 deltarealtime = realtimeseconds - LastRealTimeSeconds;
	if (!bSimIsPaused)
	{
		PausedTimeSeconds += deltarealtime;
		PausedDilatedTimeSeconds += (deltarealtime * TimeScale);
		
		int32 frames = 0;
		while (ShouldAdvanceFrame() && (frames < MaxFramesPerTick))
		{
			ERTSDKShouldAdvanceInputTurnResult advanceturn = SimState->ShouldAdvanceInputTurn();
			if (advanceturn == ERTSDKShouldAdvanceInputTurnResult::Advance)
			{
				FinalizeLockstepTurn();
			}
			else if (advanceturn == ERTSDKShouldAdvanceInputTurnResult::Wait)
			{
				SetSimIsPaused(true);
				break;
			}
			AdvanceFrame();
			frames++;
		}
	}
	else
	{
		if (SimState->GetMatchIsPaused())
		{
			if (SimState->ShouldAutoUnpause())
			{
				SimState->SetMatchIsPaused(false);
			}
		}
		ERTSDKShouldAdvanceInputTurnResult advanceturn = SimState->ShouldAdvanceInputTurn();
		if (advanceturn == ERTSDKShouldAdvanceInputTurnResult::Advance)
		{
			FinalizeLockstepTurn();
			SetSimIsPaused(false);
		}
		else if (advanceturn == ERTSDKShouldAdvanceInputTurnResult::Skip)//unpaused from match pausing
		{
			SetSimIsPaused(false);
		}
	}
	
	LastRealTimeSeconds = realtimeseconds;
}

void URTSDKGameSimSubsystem::AdvanceFrame()
{
	FrameCount++;
	GameTimeSeconds = (FRTSNumber64)FrameCount * TimeStep;
	ScriptCommandBuffer.Reset();
	ScriptCommandBuffer = MakeShareable(new FMassCommandBuffer());
	UMassProcessor& proc = *SimProcessor;
	FMassProcessingContext Context(EntityManager, TimeStep);
	Context.bFlushCommandBuffer = true;
	UE::Mass::Executor::Run(proc, Context);
	EntityManager->FlushCommands(ScriptCommandBuffer);
}

bool URTSDKGameSimSubsystem::ShouldAdvanceFrame() const
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return false;
	}

	return FRTSMath::FloorToNumber64(PausedDilatedTimeSeconds / TimeStep) > (FRTSNumber64)FrameCount;
}

bool URTSDKGameSimSubsystem::RegisterUnit(AActor* inUnitActor, URTSDKUnitComponent* inUnitComponent, FMassEntityHandle inUnitHandle, uint32& outUnitID)
{
	if ((inUnitActor == nullptr) || (!inUnitHandle.IsValid()))
	{
		return false;
	}

	uint32 newid = ClaimUnitID();
	FRTSDKRegisteredUnitInfo& newunit = UnitsByID.Add(newid);
	newunit.UnitActor = inUnitActor;
	newunit.UnitComponent = inUnitComponent;
	newunit.UnitHandle = inUnitHandle;
	outUnitID = newid;
	return true;
}

FMassCommandBuffer& URTSDKGameSimSubsystem::StartScriptCallingMode()
{
	bInScriptCallingMode = true;
	return *ScriptCommandBuffer.Get();
}

void URTSDKGameSimSubsystem::EndScriptCallingMode()
{
	bInScriptCallingMode = false;
}

void URTSDKGameSimSubsystem::SetGlobalGravityDirection(FVector inDir)
{
	UMassSignalSubsystem* SignalSubsystem = UWorld::GetSubsystem<UMassSignalSubsystem>(GetWorld());
	inDir.Normalize();
	SetGravityDirection(inDir);
	SignalSubsystem->SignalEntities(UE::Mass::Signals::RTSDKUnitGravityChanged, GetAllUnitEntityHandles());
}

bool URTSDKGameSimSubsystem::SpawnUnit(UClass* inUnitDefinition, uint32& outUnitID, int32 inForce, FRTSVector64 inSpawnLocation, FRTSRotator64 inSpawnRotation, bool bPlayAlert)
{
	FActorSpawnParameters params;
	URTSDKUnitDefinition* unitdef = inUnitDefinition->GetDefaultObject<URTSDKUnitDefinition>();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* world = GetWorld();
	AActor* unitactor = world->SpawnActor<AActor>(unitdef->UnitActor, inSpawnLocation, inSpawnRotation, params);
	if (unitactor == nullptr)
	{
		return false;
	}
	UActorComponent* comp = unitactor->GetComponentByClass(URTSDKUnitComponent::StaticClass());
	if (comp == nullptr)
	{
		unitactor->Destroy();
		return false;
	}
	URTSDKUnitComponent* unitcomp = Cast<URTSDKUnitComponent>(comp);
	if (unitcomp == nullptr)
	{
		unitactor->Destroy();
		return false;
	}
	const FMassEntityConfig& EntityConfig = unitdef->UnitConfig;
	const FMassEntityTemplate& EntityTemplate = EntityConfig.GetOrCreateEntityTemplate(*world, *unitdef);
	TArray<FMassEntityHandle> Entities;
	MassSpawnerSubsystem->SpawnEntities(EntityTemplate, 1, Entities);
	if (Entities.Num() <= 0)
	{
		unitactor->Destroy();
		return false;
	}
	FMassEntityView EntityView(EntityTemplate.GetArchetype(), Entities[0]);
	uint32 unitid = ClaimUnitID();
	unitcomp->Setup(this, unitid, inForce, inUnitDefinition, Entities[0]);
	if (EntityTemplate.GetObjectFragmentInitializers().Num())
	{
		for (const FMassEntityTemplate::FObjectFragmentInitializerFunction& Initializer : EntityTemplate.GetObjectFragmentInitializers())
		{
			Initializer(*unitactor, EntityView, EMassTranslationDirection::MassToActor);
		}
	}
	outUnitID = unitid;
	FRTSDKRegisteredUnitInfo info;
	info.UnitActor = unitactor;
	info.UnitComponent = unitcomp;
	info.UnitHandle = Entities[0];
	UnitsByID.Add(unitid, info);
	return true;
}

void URTSDKGameSimSubsystem::RebuildProcessors()
{
	//todo: mod manager needs to build a phase config from processors added by mods + the mass module settings
	//and then this needs to get that from mod manager, we call this after mods and loaded and activated but before the rest of initialization
	UWorld* world = GetWorld();
	FString DependencyGraphFileName;
	const URTSDKMassModuleSettings* Settings = GetMutableDefault<URTSDKMassModuleSettings>();
#if WITH_EDITOR

	if (Settings != nullptr && !Settings->DumpDependencyGraphFileName.IsEmpty())
	{
		DependencyGraphFileName = FString::Printf(TEXT("%s_%s"), *Settings->DumpDependencyGraphFileName, *ToString(world->GetNetMode()));
	}
#endif // WITH_EDITOR

	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();

	FMassProcessingPhaseConfig& simphase = modmanager->GetActiveSimPhaseConfig(world);

	FString FileName = !DependencyGraphFileName.IsEmpty() ? FString::Printf(TEXT("%s_%s"), *DependencyGraphFileName, *simphase.PhaseName.ToString()) : FString();
	
	SimProcessor = NewObject<UMassCompositeProcessor>(this, simphase.PhaseGroupClass,
		*FString::Printf(TEXT("ProcessingPhase_%s"), *simphase.PhaseName.ToString()));
	SimProcessor->CopyAndSort(simphase, FileName);
	SimProcessor->SetProcessingPhase(EMassProcessingPhase::PrePhysics);
	SimProcessor->SetGroupName(FName(FString::Printf(TEXT("%s Group"), *simphase.PhaseName.ToString())));
	SimProcessor->Initialize(*this);
}

bool URTSDKGameSimSubsystem::ShouldFinalizeLockstepTurn()
{
	if ((LastLockstepTurnFrame + FramesPerLockstepTurn) <= FrameCount)
	{
		return true;
	}
	return false;
}

struct FInputTurnFinalizeInfo
{
	ARTSDKCommanderStateBase* commander;
	int32 index; 
	TArray<FRTSDKPlayerCommandReplicationInfo> commands;

	FInputTurnFinalizeInfo(ARTSDKCommanderStateBase* cmdr, int32 i, TArray<FRTSDKPlayerCommandReplicationInfo> c)
		: commander(cmdr), index(i), commands(c)
	{}

	inline bool operator<(const FInputTurnFinalizeInfo& Other) const
	{
		return index < Other.index;
	}
};

void URTSDKGameSimSubsystem::FinalizeLockstepTurn()
{
	//flushes all the command buffers
	SimState->OnPreAdvanceInputTurn();
	//get this turn's commands, spawn and execute them.
	TArray<FInputTurnFinalizeInfo> infos;
	for (int32 i = 0; i < SimState->GetCommanderCount(); i++)
	{
		TArray<FRTSDKPlayerCommandReplicationInfo> commands = SimState->GetCommandsForCommanderByTurn(i, CurrentInputTurn);
		infos.Add(FInputTurnFinalizeInfo(SimState->GetCommander(i), i, commands));
	}
	infos.Sort();
	TArray<URTSDKPlayerCommandBase*> commands;
	for (int32 i = 0; i < infos.Num(); i++)
	{
		for (int32 c = 0; c < infos[i].commands.Num(); c++)
		{
			URTSDKPlayerCommandBase* command = NewObject<URTSDKPlayerCommandBase>(this, infos[i].commands[c].Class.Get());
			command->SetAll(infos[i].commander, this, infos[i].commands[c]);
			commands.Add(command);
		}
	}
	for (int32 i = 0; i < commands.Num(); i++)
	{
		commands[i]->Execute();
	}
	CurrentInputTurn++;
}
