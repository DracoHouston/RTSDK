// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RTSDKConstants.h"
#include "MassCommonTypes.h"
#include "RTSDKScriptExecutionContext.h"
#include "RTSDKGameSimSubsystem.generated.h"

class UMassSpawnerSubsystem;
class URTSDKUnitComponent;
class URTSDKPlayerCommandBase;
class ARTSDKSimStateBase;
class ARTSDKCommanderStateBase;
class ARTSDKTeamStateBase;
class ARTSDKForceStateBase;
struct FRTSDKPlayerCommandReplicationInfo;

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKRegisteredUnitInfo
{
public:
	GENERATED_BODY()

	FMassEntityHandle UnitHandle;
	TWeakObjectPtr<AActor> UnitActor;
	TWeakObjectPtr<URTSDKUnitComponent> UnitComponent;
};

/**
 * 
 */
//do we make this abstract so games have to subclass it in order to make it spawn?
//can be done for them in template projects, manual setup guide can go over it, etc
//consider it when further along
UCLASS()
class RTSDK_API URTSDKGameSimSubsystem : public UTickableWorldSubsystem
{
public:
	GENERATED_BODY()
	
	//~USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	//~End of USubsystem interface

	void SetSimIsInitialized(bool inSimIsInitialized)
	{
		bSimIsInitialized = inSimIsInitialized;
	}

	bool IsSimInitialized()
	{
		return bSimIsInitialized;
	}

	bool IsSimPaused()
	{
		return bSimIsPaused;
	}

	void SetSimIsPaused(bool inSimIsPaused)
	{
		bSimIsPaused = inSimIsPaused;
	}

	int32 GetFrameCount() const
	{
		return FrameCount;
	}

	void SetFrameCount(int32 inFrame)
	{
		FrameCount = inFrame;
	}

	void ResetFrameCount()
	{
		SetFrameCount(0);
		LastLockstepTurnFrame = FrameCount;
	}

	void AdvanceFrame();

	bool ShouldAdvanceFrame() const;

	int32 GetFramesPerLockstepTurn() const
	{
		return FramesPerLockstepTurn;
	}
	//todo, networked dilation + local dilation, scaling thisby networked dilation
	void SetFramesPerLockstepTurn(int32 inFrameCount)
	{
		FramesPerLockstepTurn = inFrameCount;
	}

	int32 GetCurrentInputTurn() const
	{
		return CurrentInputTurn;
	}

	void SetCurrentInputTurn(int32 inTurnCount)
	{
		CurrentInputTurn = inTurnCount;
	}	

	FRTSNumber64 GetTimestep() const
	{
		return TimeStep;
	}
	
	int32 GetTargetUPS()
	{
		return TargetUPS;
	}

	void SetTargetUPS(int32 inTargetUPS)
	{
		TargetUPS = inTargetUPS;
		TimeStep = TargetUPS > 0 ? FRTSNumber64::Make(1.0) / (FRTSNumber64)TargetUPS : FRTSNumber64::Make(0.0);
		SetGravityAcceleration(GravityAccelerationMeters);
		SetTerminalVelocity(TerminalVelocityMeters);
	}

	int32 GetMaxFramesPerTick() const
	{
		return MaxFramesPerTick;
	}

	void SetMaxFramesPerTick(int32 inFramesPerTick)
	{
		MaxFramesPerTick = inFramesPerTick;
	}

	FRTSNumber64 GetTimeScale() const
	{
		return TimeScale;
	}

	void SetTimeScale(FRTSNumber64 inTimeScale)
	{
		TimeScale = inTimeScale;
	}
	
	FRTSNumber64 GetMetersToUUScale() const
	{
		return MetersToUUScale;
	}

	void SetMetersToUUScale(FRTSNumber64 inMetersToUUScale)
	{
		MetersToUUScale = inMetersToUUScale;
		SetGravityAcceleration(GravityAccelerationMeters);
		SetTerminalVelocity(TerminalVelocityMeters);
	}

	FRTSNumber64 GetGravityAcceleration() const
	{
		return GravityAcceleration;
	}

	void SetGravityAcceleration(FRTSNumber64 inGravityAcceleration)
	{
		GravityAccelerationMeters = inGravityAcceleration;
		GravityAcceleration = (GravityAccelerationMeters * MetersToUUScale) * TimeStep;
		GravityVector = GravityAcceleration > FRTSNumber64::Make(0.0) ? GravityDirection * GravityAcceleration : FRTSVector64::ZeroVector;
	}

	FRTSNumber64 GetTerminalVelocity() const
	{
		return TerminalVelocity;
	}

	void SetTerminalVelocity(FRTSNumber64 inTerminalVelocity)
	{
		TerminalVelocityMeters = inTerminalVelocity;
		TerminalVelocity = (TerminalVelocityMeters * MetersToUUScale) * TimeStep;
	}

	FRTSVector64 GetGravityDirection() const
	{
		return GravityDirection;
	}

	FRTSVector64 GetGravityVector() const
	{
		return GravityVector;
	}

	void SetGravityDirection(FRTSVector64 inGravityDirection)
	{
		GravityDirection = inGravityDirection;
		GravityVector = GravityAcceleration > FRTSNumber64::Make(0.0) ? GravityDirection * GravityAcceleration : FRTSVector64::ZeroVector;
	}
		
	bool RegisterUnit(AActor* inUnitActor, URTSDKUnitComponent* inUnitComponent, FMassEntityHandle inUnitHandle, uint32& outUnitID);

	bool DoesUnitExist(int64 TargetID) const
	{
		return UnitsByID.Contains(TargetID);
	}

	const FRTSDKRegisteredUnitInfo& GetUnitInfoByIDChecked(int64 TargetID) const
	{
		return UnitsByID.FindChecked(TargetID);
	}

	const FRTSDKRegisteredUnitInfo* GetUnitInfoByID(int64 TargetID) const
	{
		return UnitsByID.Find(TargetID);
	}

	FMassCommandBuffer& StartScriptCallingMode();
	void EndScriptCallingMode();

	bool IsScriptCallingMode()
	{
		return bInScriptCallingMode;
	}

	UFUNCTION(BlueprintCallable)
	void SetGlobalGravityDirection(FVector inDir);

	ARTSDKSimStateBase* GetSimState()
	{
		return SimState;
	}
	
	void SetSimState(ARTSDKSimStateBase* inSimState)
	{
		SimState = inSimState;
	}

	bool SpawnUnit(UClass* inUnitDefinition, uint32& outUnitID, int32 inForce, FRTSVector64 inSpawnLocation, FRTSRotator64 inSpawnRotation = FRTSRotator64::ZeroRotator, bool bPlayAlert = true);

	TSharedPtr<FMassEntityManager> GetEntityManager()
	{
		return EntityManager;
	}

protected:
	bool bSimIsInitialized;
	bool bLevelUnitsInitialized;
	bool bSimIsPaused;
	bool bInScriptCallingMode;
	int32 FrameCount;
	int32 TargetUPS;
	int32 MaxFramesPerTick;
	FRTSNumber64 TimeStep;
	FRTSNumber64 TimeScale;
	FRTSNumber64 LastRealTimeSeconds;
	FRTSNumber64 TotalPauseDuration;
	FRTSNumber64 PausedTimeSeconds;
	FRTSNumber64 PausedDilatedTimeSeconds;
	FRTSNumber64 GameTimeSeconds;

	UPROPERTY(EditAnywhere, Category = Mass)
	TObjectPtr<UMassCompositeProcessor> SimProcessor = nullptr;

	TSharedPtr<FMassEntityManager> EntityManager;

	TObjectPtr<UMassSpawnerSubsystem> MassSpawnerSubsystem;

	TObjectPtr<AWorldSettings> WorldSettings;

	TObjectPtr<ARTSDKSimStateBase> SimState;

	int32 CurrentInputTurn;
	int32 LastLockstepTurnFrame;
	int32 FramesPerLockstepTurn;
	FRTSNumber64 LockstepTurnTimeStep;
	
	TMap<uint32, FRTSDKRegisteredUnitInfo> UnitsByID;
	TSharedPtr<FMassCommandBuffer> ScriptCommandBuffer;

	//TMap<TSubclassOf<FRTSBatchedSimCommand>, TArray<TWeakPtr<FRTSBatchedSimCommand>>> SimCommands;
	uint32 NextUnitID;
	int64 NextPlayerID;
	
	FRTSNumber64 MetersToUUScale;
	FRTSNumber64 GravityAcceleration;
	FRTSNumber64 GravityAccelerationMeters;
	FRTSNumber64 TerminalVelocity;
	FRTSNumber64 TerminalVelocityMeters;
	FRTSVector64 GravityDirection;
	FRTSVector64 GravityVector;

	void SetTimestep(FRTSNumber64 inTimeStep)
	{
		TimeStep = inTimeStep;
	}

	bool ShouldFinalizeLockstepTurn();

	void FinalizeLockstepTurn();

	TArray<FMassEntityHandle> GetAllUnitEntityHandles()
	{
		TArray<FMassEntityHandle> retval;
		retval.Empty(UnitsByID.Num());
		for (auto It = UnitsByID.CreateConstIterator(); It; ++It)
		{
			retval.Add(It->Value.UnitHandle);
		}
		return retval;
	}

	void ResetUnits()
	{
		NextUnitID = 0;
		UnitsByID.Empty();
	}
	
	uint32 ClaimUnitID()
	{
		uint32 retval = NextUnitID;
		NextUnitID++;
		return retval;
	}
};
