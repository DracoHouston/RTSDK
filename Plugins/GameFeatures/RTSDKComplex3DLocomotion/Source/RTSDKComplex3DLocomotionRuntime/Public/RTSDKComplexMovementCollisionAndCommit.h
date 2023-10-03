// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSignalProcessorBase.h"
#include "RTSDKGameSimProcessorBase.h"
#include "RTSDKGameSimProcessorInterface.h"
#include "RTSDKFragments.h"
#include "RTSDKMovementCoreFragments.h"
#include "RTSDKComplexMovementFragments.h"
#include "RTSDKComplexMovementCollisionAndCommit.generated.h"

struct FBasedWalkingMoveCommitInfo
{
	uint32 Key;
	
	FRTSCurrentLocationFragment& Location;
	FRTSCurrentRotationFragment& Rotation;
	FRTSVelocityFragment& Velocity;
	FRTSMovementBasisFragment& Basis;

	const FRTSCollisionBoundsFragment& Bounds;
	const FRTSMovementComplexWalkingParamsFragment& WalkingParams;

	FMassEntityHandle Entity;

	USceneComponent* SimRoot;
	URTSDKUnitComponent* UnitComponent;

	FBasedWalkingMoveCommitInfo(
		USceneComponent* inRoot, 
		URTSDKUnitComponent* inUnit, 
		FRTSCurrentLocationFragment& inLocFragment, 
		FRTSCurrentRotationFragment& inRotFragment, 
		FRTSVelocityFragment& inVelocityFragment,
		FRTSMovementBasisFragment& inBasis, 
		const FRTSCollisionBoundsFragment& inBounds, 
		const FRTSMovementComplexWalkingParamsFragment& inMovementComplexWalkingParams, 
		int64 UnitID, 
		FMassEntityHandle inEntity)
		: Location(inLocFragment), 
		Rotation(inRotFragment), 
		Velocity(inVelocityFragment), 
		Basis(inBasis), 
		Bounds(inBounds), 
		WalkingParams(inMovementComplexWalkingParams), 
		Entity(inEntity), 
		SimRoot(inRoot), 
		UnitComponent(inUnit)
	{
		Key = UnitID;
	}

	inline bool operator<(const FBasedWalkingMoveCommitInfo& Other) const
	{
		return Key < Other.Key;
	}
};

struct FAirWalkingMoveCommitInfo
{
	uint32 Key;

	FRTSCurrentLocationFragment& Location;
	FRTSCurrentRotationFragment& Rotation;
	FRTSVelocityFragment& Velocity;

	const FRTSCollisionBoundsFragment& Bounds;
	const FRTSMovementComplexWalkingParamsFragment& WalkingParams;

	FMassEntityHandle Entity;

	USceneComponent* SimRoot;
	URTSDKUnitComponent* UnitComponent;

	FAirWalkingMoveCommitInfo(
		USceneComponent* inRoot, 
		URTSDKUnitComponent* inUnit, 
		FRTSCurrentLocationFragment& inLocFragment, 
		FRTSCurrentRotationFragment& inRotFragment, 
		FRTSVelocityFragment& inVelocityFragment, 
		const FRTSCollisionBoundsFragment& inBounds, 
		const FRTSMovementComplexWalkingParamsFragment& inMovementComplexParams, 
		int64 UnitID, 
		FMassEntityHandle inEntity)
		: Location(inLocFragment), 
		Rotation(inRotFragment), 
		Velocity(inVelocityFragment), 
		Bounds(inBounds), 
		WalkingParams(inMovementComplexParams), 
		Entity(inEntity), 
		SimRoot(inRoot), 
		UnitComponent(inUnit)
	{
		//todo: make a 64 bit hash out of this, frame count and some constant
		Key = UnitID;
	}

	inline bool operator<(const FAirWalkingMoveCommitInfo& Other) const
	{
		return Key < Other.Key;
	}
};

struct FComplexMovementRotationCommitInfo
{
	uint32 Key;

	FRTSCurrentLocationFragment& Location;
	FRTSCurrentRotationFragment& Rotation;
	FRTSAngularVelocityFragment& Velocity;

	const FRTSCollisionBoundsFragment& Bounds;

	FMassEntityHandle Entity;

	USceneComponent* SimRoot;

	FComplexMovementRotationCommitInfo(
		USceneComponent* inRoot,
		FRTSCurrentLocationFragment& inLocFragment,
		FRTSCurrentRotationFragment& inRotFragment,
		FRTSAngularVelocityFragment& inAngularVelocityFragment,
		const FRTSCollisionBoundsFragment& inBounds,
		int64 UnitID,
		FMassEntityHandle inEntity)
		: Location(inLocFragment),
		Rotation(inRotFragment),
		Velocity(inAngularVelocityFragment),
		Bounds(inBounds),
		Entity(inEntity),
		SimRoot(inRoot)
	{
		//todo: make a 64 bit hash out of this, frame count and some constant
		Key = UnitID;
	}

	inline bool operator<(const FComplexMovementRotationCommitInfo& Other) const
	{
		return Key < Other.Key;
	}
};

/**
*
*/
UCLASS()
class URTSDKComplexMovementCommit : public URTSDKModGameSimProcessorBase
{
	GENERATED_BODY()
public:
	URTSDKComplexMovementCommit();
	
protected:
	virtual void ConfigureQueries() override;
	//virtual void SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals) override;

	void ProcessBasedMovers(FMassEntityManager& EntityManager, FMassExecutionContext& Context);

	void ProcessAirMovers(FMassEntityManager& EntityManager, FMassExecutionContext& Context);

	bool UpdateMovementBase() {}

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	FMassEntityQuery BasedMoversQuery;
	FMassEntityQuery AirMoversQuery;
	TArray<FBasedWalkingMoveCommitInfo> BasedMoveCommitsThisFrame;
	TArray<FAirWalkingMoveCommitInfo> AirMoveCommitsThisFrame;
};

/**
*
*/
UCLASS()
class URTSDKComplexRotationCommit : public URTSDKModGameSimProcessorBase
{
	GENERATED_BODY()
public:
	URTSDKComplexRotationCommit();

protected:
	virtual void ConfigureQueries() override;

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	FMassEntityQuery RotatorsQuery;
	TArray<FComplexMovementRotationCommitInfo> RotationCommitsThisFrame;
};
