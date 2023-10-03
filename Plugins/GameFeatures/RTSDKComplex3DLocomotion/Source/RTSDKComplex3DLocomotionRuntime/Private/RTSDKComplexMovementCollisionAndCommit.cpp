// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKComplexMovementCollisionAndCommit.h"
#include "RTSDKGameSimSubsystem.h"
#include "MassSignalSubsystem.h"
#include "RTSDKConstants.h"
#include "RTSDKBatchedSimCommand.h"
#include "RTSDKUnitComponent.h"

URTSDKComplexMovementCommit::URTSDKComplexMovementCommit()
{
	bRequiresGameThreadExecution = true;
	RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_COMPLEX_LOCATION_COMMIT
}

URTSDKComplexRotationCommit::URTSDKComplexRotationCommit()
{
	bRequiresGameThreadExecution = true;
	RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_COMPLEX_ROTATION_COMMIT
}

void URTSDKComplexMovementCommit::ConfigureQueries()
{
	BasedMoversQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSCollisionBoundsFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSUnitComponentFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSMovementComplexWalkingParamsFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	BasedMoversQuery.AddRequirement<FRTSMovementBasisFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	BasedMoversQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);

	BasedMoversQuery.RegisterWithProcessor(*this);

	AirMoversQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSCollisionBoundsFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSUnitComponentFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSMovementComplexWalkingParamsFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	AirMoversQuery.AddRequirement<FRTSMovementBasisFragment>(EMassFragmentAccess::None, EMassFragmentPresence::None);
	AirMoversQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);
	
	AirMoversQuery.RegisterWithProcessor(*this);
}

void URTSDKComplexRotationCommit::ConfigureQueries()
{
	RotatorsQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	RotatorsQuery.AddRequirement<FRTSCollisionBoundsFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	RotatorsQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	RotatorsQuery.AddRequirement<FRTSUnitComponentFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	RotatorsQuery.AddRequirement<FRTSAngularVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	RotatorsQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	RotatorsQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	RotatorsQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);

	RotatorsQuery.RegisterWithProcessor(*this);
}

//void URTSDKComplexMovementCommit::SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals)
//{
//	
//	//TODO
//	//add penetration check on no collision when based moving, against just the movement base's primitive component
//	//look into a grid-snap system for collision results:
//	//standard 'collision grid' of some power of 2 fraction of 1, such as 32, 64, 128 etc
//	//use the grid snap function in frtsmath to do it under the hood, helper should exist for this
//	//helper eats a hit result returns a rts hit result using deterministic math types
//	//work out the distance between 2 points with FRTSVector64s and dont trust hitresult.distance
//	
//}

//Sweeps capsule conforming to bounds along velocity.
//Any collider with the BlocksUnits channel set to block will restrict the unit if hit.
//Their velocity will be recalculated and a collision event will be queued on the unit
void SweepAgainstBlocksUnits(
	UWorld* inWorld,
	URTSDKUnitComponent* inUnit,
	const FCollisionShape& inCollisionShape,
	const FMassEntityHandle& inEntity,
	const FRTSVector64& inLocation, 
	const FRTSQuat64& inRotation, 
	FRTSVector64& outNewLocation,
	FRTSVelocityFragment& outVelocityFragment,
	bool& outCollided,
	FHitResult& outCollisionResult)
{
	FHitResult Hit;
	outNewLocation = inLocation + outVelocityFragment.Velocity;
	FCollisionQueryParams collisionqueryparams;
	collisionqueryparams.AddIgnoredActor(inUnit->GetOwner());
	collisionqueryparams.bIgnoreTouches = true;//blocking only

	if (inWorld->SweepSingleByChannel
	(
		Hit,
		inLocation,
		outNewLocation,
		inRotation,
		ECollisionChannel::ECC_GameTraceChannel2,
		inCollisionShape,
		collisionqueryparams
	))
	{
		outVelocityFragment.Velocity = FMath::IsNearlyZero(Hit.Distance) ? FRTSVector64::ZeroVector : outVelocityFragment.Velocity.GetSafeNormal() * ((FRTSNumber64)Hit.Distance * FRTSNumber64::Make(0.95));
		outNewLocation = inLocation + outVelocityFragment.Velocity;
		outCollided = true;
		outCollisionResult = Hit;
	}
}

//Checks the floor angle against the gravity direction.
//Returns true if the walkable angle is less than or equal to the angle.
//Walkable angle is assumed to be the cos of the walkable angle.
bool FloorAngleCheck(const FRTSVector64& inGravityDirection, const FRTSVector64& inFloorNormal, const FRTSNumber64& inWalkableAngle, FRTSNumber64& outFloorAngle)
{
	outFloorAngle = FRTSMath::Acos(-inGravityDirection | inFloorNormal);
	return outFloorAngle <= inWalkableAngle;
}

bool RetryFloor(
	UPrimitiveComponent* inComponent,
	const FRTSVector64& inGravityDirection,
	const FCollisionShape& inCollisionShape,
	const FRTSVector64& inPosition,
	const FRTSQuat64& inRotation, 
	const FRTSVector64& inStepUpOffset, 
	const FRTSVector64& inStepDownOffset,
	const FRTSNumber64& inWalkableAngle, 
	FRTSNumber64& outFloorAngle,
	FHitResult& outHitResult,
	bool& outFailedPenetrationTest)
{
	FRTSVector64 testpos = inPosition;
	FRTSVector64 teststart = testpos + inStepUpOffset;
	FRTSVector64 testend = testpos + inStepDownOffset;
	outFailedPenetrationTest = false;
	if (inComponent->SweepComponent(outHitResult, teststart, testend, inRotation, inCollisionShape))
	{
		if (!outHitResult.bStartPenetrating)
		{
			return FloorAngleCheck(inGravityDirection, (FRTSVector64)outHitResult.ImpactNormal, inWalkableAngle, outFloorAngle);
		}
		outFailedPenetrationTest = true;
	}
	return false;
}

void UpdateFloor(
	UWorld* inWorld,
	URTSDKUnitComponent* inUnit,
	UPrimitiveComponent* inOldBase,
	const FCollisionShape& inCollisionShape,
	const FRTSVector64& inGravityDirection, 
	const FRTSVector64& inFeetLocation,
	const FRTSMovementComplexWalkingParamsFragment& inWalkingParams,
	const FRTSVector64& inCurrentLocation,
	const FRTSQuat64& inRotation,
	FRTSVelocityFragment& outVelocityFragment,
	FRTSVector64& outNewLocation,
	bool& outFoundBase,
	FHitResult& outBestFloorResult,
	bool& outCollidedWithUnsuitableBase,
	FHitResult& outCollisionResult)
{
	FRTSVector64 stepupoffset = -inGravityDirection * inWalkingParams.StepUpHeight;
	FRTSVector64 stepdownoffset = inGravityDirection * inWalkingParams.StepDownHeight;

	FRTSVector64 start = outNewLocation + stepupoffset;
	FRTSVector64 end = outNewLocation + stepdownoffset;
	
	TArray<FHitResult> BasisHits;
	FCollisionQueryParams collisionqueryparams;
	collisionqueryparams.AddIgnoredActor(inUnit->GetOwner());
	collisionqueryparams.bIgnoreTouches = true;//blocking only

	FRTSNumber64 bestfloorangle;
	FRTSNumber64 bestfloordistance;
	TArray<UPrimitiveComponent*> unsuitablebases;
	//sweep between step up and step down locations against WalkableTerrain channel
	if (inWorld->SweepMultiByChannel
	(
		BasisHits,
		start,
		end,
		inRotation,
		ECollisionChannel::ECC_GameTraceChannel1,
		inCollisionShape,
		collisionqueryparams))
	{
		//First blocking hit is used if nothing better is found, and valid. It is last in the array of hits.
		outBestFloorResult = BasisHits[BasisHits.Num() - 1];
		//if it is penetrating and we have a depenetration solution
		if ((outBestFloorResult.bStartPenetrating) && (outBestFloorResult.PenetrationDepth > 0.0))
		{
			//retest against just the component, from suggested depenetration position
			FRTSVector64 testpos = (FRTSVector64)outBestFloorResult.Location + ((FRTSVector64)outBestFloorResult.Normal * (FRTSNumber64)outBestFloorResult.PenetrationDepth);
			FRTSNumber64 floorangle = 0.0;
			FHitResult retryhit;
			bool failedpenetrationtest;
			if (RetryFloor(outBestFloorResult.GetComponent(), 
				inGravityDirection, 
				inCollisionShape, 
				testpos, 
				inRotation, 
				stepupoffset, 
				stepdownoffset, 
				inWalkingParams.MaxWalkableAngle, 
				floorangle, 
				retryhit,
				failedpenetrationtest))
			{
				//valid, setting as best
				outBestFloorResult = retryhit;
				bestfloordistance = (testpos - outBestFloorResult.Location).SizeSquared();
				bestfloorangle = floorangle;
				outFoundBase = true;
			}
			else
			{
				//invalid, adding for collision
				unsuitablebases.Add(outBestFloorResult.GetComponent());
			}			
		}
		else //not penetrating
		{
			//Is the floor valid for walking angle?
			FRTSNumber64 floorangle;
			if (FloorAngleCheck(inGravityDirection, (FRTSVector64)outBestFloorResult.ImpactNormal, inWalkingParams.MaxWalkableAngle, floorangle))
			{
				//yes
				bestfloordistance = (outNewLocation - outBestFloorResult.Location).SizeSquared();
				bestfloorangle = floorangle;
				outFoundBase = true;
			}
			else
			{
				//no, adding for collision
				unsuitablebases.Add(outBestFloorResult.GetComponent());
			}
		}
		//For each other hit in the multi sweep we retry against the component
		for (int32 hitsidx = BasisHits.Num() - 2; hitsidx >= 0; hitsidx--)
		{
			FRTSNumber64 floorangle = 0.0;
			FHitResult otherhit;
			bool failedpenetrationtest;
			UPrimitiveComponent* testedcomponent = BasisHits[hitsidx].GetComponent();
			if (RetryFloor(
				testedcomponent,
				inGravityDirection, 
				inCollisionShape, 
				outNewLocation, 
				inRotation, 
				stepupoffset, 
				stepdownoffset, 
				inWalkingParams.MaxWalkableAngle, 
				floorangle, 
				otherhit,
				failedpenetrationtest))
			{
				//valid, closer than current best?
				FRTSNumber64 dist = (outNewLocation - otherhit.Location).SizeSquared();
				if (dist < bestfloordistance)
				{
					//setting as best
					outBestFloorResult = otherhit;
					bestfloordistance = dist;
					bestfloorangle = floorangle;
					outFoundBase = true;
				}
			}
			else 
			{
				//if we failed because of penetration test and not because of walkable angle, and there is a depenetration solution, retry there
				if (failedpenetrationtest && (otherhit.PenetrationDepth > 0.0))
				{
					FRTSVector64 testpos = (FRTSVector64)otherhit.Location + ((FRTSVector64)otherhit.Normal * (FRTSNumber64)otherhit.PenetrationDepth);
					FHitResult retryhit;
					if (RetryFloor(otherhit.GetComponent(),
						inGravityDirection,
						inCollisionShape,
						testpos,
						inRotation,
						stepupoffset,
						stepdownoffset,
						inWalkingParams.MaxWalkableAngle,
						floorangle,
						retryhit,
						failedpenetrationtest))
					{
						//valid, closer?
						FRTSNumber64 dist = (outNewLocation - retryhit.Location).SizeSquared();
						if (dist < bestfloordistance)
						{
							//setting as best
							outBestFloorResult = retryhit;
							bestfloordistance = dist;
							bestfloorangle = floorangle;
							outFoundBase = true;
						}
					}
					else
					{
						//invalid, adding for collision
						unsuitablebases.Add(testedcomponent);
					}
				}
				else
				{
					//unwalkable, collide with it
					unsuitablebases.Add(testedcomponent);
				}
			}
		}

		if (outFoundBase && (outBestFloorResult.Component != inOldBase))
		{
			outNewLocation = outBestFloorResult.Location;// +-inFeetLocation;
		}

		//for each unwalkable WalkableTerrain component hit, sweep them as collision
		FHitResult Hit;
		outCollidedWithUnsuitableBase = false;
		FRTSNumber64 closestdistance = UE_DOUBLE_BIG_NUMBER;
		FHitResult closestblockingunsuitablebasehit;
		for (int32 blockersidx = 0; blockersidx < unsuitablebases.Num(); blockersidx++)
		{
			if (unsuitablebases[blockersidx]->SweepComponent(
				Hit,
				inCurrentLocation,
				outNewLocation,
				inRotation,
				inCollisionShape))
			{
				if ((FRTSNumber64)Hit.Distance < closestdistance)
				{
					closestdistance = Hit.Distance;
					outCollisionResult = Hit;
					outCollidedWithUnsuitableBase = true;
				}
			}
		}
		if (outCollidedWithUnsuitableBase)
		{
			outVelocityFragment.Velocity = FRTSMath::IsNearlyZero(closestdistance) ? FRTSVector64::ZeroVector : outVelocityFragment.Velocity.GetSafeNormal() * (closestdistance * FRTSNumber64::Make(0.95));
			outNewLocation = inCurrentLocation + outVelocityFragment.Velocity;
		}
	}
}

void URTSDKComplexMovementCommit::ProcessBasedMovers(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	UWorld* world = GetWorld();
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	FRTSVector64 gravdir = sim->GetGravityDirection();
	for (int32 i = 0; i < BasedMoveCommitsThisFrame.Num(); i++)
	{
		FCollisionShape boundsbox = FCollisionShape::MakeCapsule(BasedMoveCommitsThisFrame[i].Bounds.BoundsRadius, BasedMoveCommitsThisFrame[i].Bounds.BoundsHalfHeight);
		FRTSVector64 testlocation;
		FRTSQuat64 rotquat = BasedMoveCommitsThisFrame[i].Rotation.Rotation.Quaternion();
		bool collidedblocksunits = false;
		FHitResult blocksunitshit;
		SweepAgainstBlocksUnits(
			world,
			BasedMoveCommitsThisFrame[i].UnitComponent,
			boundsbox,
			BasedMoveCommitsThisFrame[i].Entity,
			BasedMoveCommitsThisFrame[i].Location.Location, 
			rotquat,
			testlocation,
			BasedMoveCommitsThisFrame[i].Velocity,
			collidedblocksunits,
			blocksunitshit);
		bool foundbase = false;
		bool collidedwithunsuitablebase = false;
		FHitResult floorresult;
		FHitResult basecollision;
		//FCollisionShape basisboundsbox = FCollisionShape::MakeCapsule(BasedMoveCommitsThisFrame[i].Bounds.BoundsRadius, 0.5);
		UpdateFloor(
			world,
			BasedMoveCommitsThisFrame[i].UnitComponent,
			BasedMoveCommitsThisFrame[i].Basis.Basis.Get(),
			boundsbox,
			gravdir,
			BasedMoveCommitsThisFrame[i].Bounds.FeetLocation,
			BasedMoveCommitsThisFrame[i].WalkingParams,
			BasedMoveCommitsThisFrame[i].Location.Location,
			BasedMoveCommitsThisFrame[i].Rotation.Rotation.Quaternion(),
			BasedMoveCommitsThisFrame[i].Velocity,
			testlocation,
			foundbase,
			floorresult,
			collidedwithunsuitablebase,
			basecollision);

		if (foundbase)
		{
			BasedMoveCommitsThisFrame[i].Basis.Basis = floorresult.Component;
			BasedMoveCommitsThisFrame[i].Basis.Impact = floorresult;
		}
		else
		{
			Context.Defer().RemoveFragment<FRTSMovementBasisFragment>(BasedMoveCommitsThisFrame[i].Entity);
			BasedMoveCommitsThisFrame[i].SimRoot->ComponentTags.Add(RTSDK::FallingUnitComponentTag);
		}
		
		if (collidedwithunsuitablebase)
		{
			Context.Defer().PushCommand<FRTSDKBroadcastUnitOnCollidedCommand>(BasedMoveCommitsThisFrame[i].Entity, BasedMoveCommitsThisFrame[i].UnitComponent, basecollision);
		}
		else if (collidedblocksunits)
		{
			Context.Defer().PushCommand<FRTSDKBroadcastUnitOnCollidedCommand>(BasedMoveCommitsThisFrame[i].Entity, BasedMoveCommitsThisFrame[i].UnitComponent, blocksunitshit);
		}

		BasedMoveCommitsThisFrame[i].SimRoot->SetWorldLocation(testlocation);
		BasedMoveCommitsThisFrame[i].SimRoot->ComponentVelocity = BasedMoveCommitsThisFrame[i].Velocity.Velocity;
		BasedMoveCommitsThisFrame[i].Location.Location = testlocation;
	}
}

void URTSDKComplexMovementCommit::ProcessAirMovers(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	UWorld* world = GetWorld();
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	FRTSVector64 gravdir = sim->GetGravityDirection();
	for (int32 i = 0; i < AirMoveCommitsThisFrame.Num(); i++)
	{
		FCollisionShape boundsbox = FCollisionShape::MakeCapsule(AirMoveCommitsThisFrame[i].Bounds.BoundsRadius, AirMoveCommitsThisFrame[i].Bounds.BoundsHalfHeight);
		FRTSVector64 testlocation;
		FRTSQuat64 rotquat = AirMoveCommitsThisFrame[i].Rotation.Rotation.Quaternion();
		bool collidedblocksunits = false;
		FHitResult blocksunitshit;
		SweepAgainstBlocksUnits(
			world,
			AirMoveCommitsThisFrame[i].UnitComponent,
			boundsbox,
			AirMoveCommitsThisFrame[i].Entity,
			AirMoveCommitsThisFrame[i].Location.Location,
			rotquat,
			testlocation,
			AirMoveCommitsThisFrame[i].Velocity,
			collidedblocksunits,
			blocksunitshit);
		bool foundbase = false;
		bool collidedwithunsuitablebase = false;
		FHitResult floorresult;
		FHitResult basecollision;
		//FCollisionShape basisboundsbox = FCollisionShape::MakeCapsule(AirMoveCommitsThisFrame[i].Bounds.BoundsRadius, 0.5);
		UpdateFloor(
			world,
			AirMoveCommitsThisFrame[i].UnitComponent,
			nullptr,
			boundsbox,
			gravdir,
			AirMoveCommitsThisFrame[i].Bounds.FeetLocation,
			AirMoveCommitsThisFrame[i].WalkingParams,
			AirMoveCommitsThisFrame[i].Location.Location,
			AirMoveCommitsThisFrame[i].Rotation.Rotation.Quaternion(),
			AirMoveCommitsThisFrame[i].Velocity,
			testlocation,
			foundbase,
			floorresult,
			collidedwithunsuitablebase,
			basecollision);

		if (foundbase)
		{
			FRTSMovementBasisFragment basis;
			basis.Basis = floorresult.Component;
			basis.Impact = floorresult;
			Context.Defer().PushCommand<FMassCommandAddFragmentInstances>(AirMoveCommitsThisFrame[i].Entity, basis);
			AirMoveCommitsThisFrame[i].Velocity.Velocity = FRTSVector64::ZeroVector;
			AirMoveCommitsThisFrame[i].SimRoot->ComponentTags.Remove(RTSDK::FallingUnitComponentTag);
		}

		if (collidedwithunsuitablebase)
		{
			Context.Defer().PushCommand<FRTSDKBroadcastUnitOnCollidedCommand>(AirMoveCommitsThisFrame[i].Entity, AirMoveCommitsThisFrame[i].UnitComponent, basecollision);
		}
		else if (collidedblocksunits)
		{
			Context.Defer().PushCommand<FRTSDKBroadcastUnitOnCollidedCommand>(AirMoveCommitsThisFrame[i].Entity, AirMoveCommitsThisFrame[i].UnitComponent, blocksunitshit);
		}

		AirMoveCommitsThisFrame[i].SimRoot->SetWorldLocation(testlocation);
		AirMoveCommitsThisFrame[i].SimRoot->ComponentVelocity = AirMoveCommitsThisFrame[i].Velocity.Velocity;
		AirMoveCommitsThisFrame[i].Location.Location = testlocation;
	}
}

void URTSDKComplexMovementCommit::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	BasedMoveCommitsThisFrame.Empty();
	AirMoveCommitsThisFrame.Empty();
	
	BasedMoversQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TConstArrayView<FRTSSimRootFragment> roots = Context.GetFragmentView<FRTSSimRootFragment>();
		TConstArrayView<FRTSUnitComponentFragment> unitcomps = Context.GetFragmentView<FRTSUnitComponentFragment>();
		TConstArrayView<FRTSUnitIDFragment> unitids = Context.GetFragmentView<FRTSUnitIDFragment>();
		TConstArrayView<FRTSCollisionBoundsFragment> bounds = Context.GetFragmentView<FRTSCollisionBoundsFragment>();
		TConstArrayView<FRTSMovementComplexWalkingParamsFragment> complexparams = Context.GetFragmentView<FRTSMovementComplexWalkingParamsFragment>();
		TConstArrayView<FMassEntityHandle> entities = Context.GetEntities();
		TArrayView<FRTSVelocityFragment> velocities = Context.GetMutableFragmentView<FRTSVelocityFragment>();
		TArrayView<FRTSCurrentLocationFragment> locations = Context.GetMutableFragmentView<FRTSCurrentLocationFragment>();
		TArrayView<FRTSCurrentRotationFragment> rotations = Context.GetMutableFragmentView<FRTSCurrentRotationFragment>();
		TArrayView<FRTSMovementBasisFragment> bases = Context.GetMutableFragmentView<FRTSMovementBasisFragment>();
		int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			if (velocities[i].Velocity.IsNearlyZero())
			{
				continue;
			}
			FBasedWalkingMoveCommitInfo movecommit(
				roots[i].SimRoot.Get(), 
				unitcomps[i].UnitComponent.Get(), 
				locations[i], 
				rotations[i], 
				velocities[i], 
				bases[i], 
				bounds[i], 
				complexparams[i], 
				unitids[i].UnitID, 
				entities[i]);
			BasedMoveCommitsThisFrame.Add(movecommit);
		}
	});
	AirMoversQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TConstArrayView<FRTSSimRootFragment> roots = Context.GetFragmentView<FRTSSimRootFragment>();
		TConstArrayView<FRTSUnitComponentFragment> unitcomps = Context.GetFragmentView<FRTSUnitComponentFragment>();
		TConstArrayView<FRTSUnitIDFragment> unitids = Context.GetFragmentView<FRTSUnitIDFragment>();
		TConstArrayView<FRTSCollisionBoundsFragment> bounds = Context.GetFragmentView<FRTSCollisionBoundsFragment>();
		TConstArrayView<FRTSMovementComplexWalkingParamsFragment> complexparams = Context.GetFragmentView<FRTSMovementComplexWalkingParamsFragment>();
		TConstArrayView<FMassEntityHandle> entities = Context.GetEntities();
		TArrayView<FRTSVelocityFragment> velocities = Context.GetMutableFragmentView<FRTSVelocityFragment>();
		TArrayView<FRTSCurrentLocationFragment> locations = Context.GetMutableFragmentView<FRTSCurrentLocationFragment>();
		TArrayView<FRTSCurrentRotationFragment> rotations = Context.GetMutableFragmentView<FRTSCurrentRotationFragment>();
		int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			if (velocities[i].Velocity.IsNearlyZero())
			{
				continue;
			}
			FAirWalkingMoveCommitInfo movecommit(
				roots[i].SimRoot.Get(), 
				unitcomps[i].UnitComponent.Get(), 
				locations[i], 
				rotations[i], 
				velocities[i],
				bounds[i], 
				complexparams[i], 
				unitids[i].UnitID, 
				entities[i]);
			AirMoveCommitsThisFrame.Add(movecommit);
		}
	});	

	BasedMoveCommitsThisFrame.Sort();
	AirMoveCommitsThisFrame.Sort();
	
	ProcessBasedMovers(EntityManager, Context);
	ProcessAirMovers(EntityManager, Context);
}

void URTSDKComplexRotationCommit::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	RotationCommitsThisFrame.Empty();
	
	RotatorsQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TConstArrayView<FRTSSimRootFragment> roots = Context.GetFragmentView<FRTSSimRootFragment>();
		TConstArrayView<FRTSUnitIDFragment> unitids = Context.GetFragmentView<FRTSUnitIDFragment>();
		TConstArrayView<FRTSCollisionBoundsFragment> bounds = Context.GetFragmentView<FRTSCollisionBoundsFragment>();
		TConstArrayView<FMassEntityHandle> entities = Context.GetEntities();
		TArrayView<FRTSAngularVelocityFragment> velocities = Context.GetMutableFragmentView<FRTSAngularVelocityFragment>();
		TArrayView<FRTSCurrentLocationFragment> locations = Context.GetMutableFragmentView<FRTSCurrentLocationFragment>();
		TArrayView<FRTSCurrentRotationFragment> rotations = Context.GetMutableFragmentView<FRTSCurrentRotationFragment>();
		int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			if (velocities[i].AngularVelocity.IsNearlyZero())
			{
				continue;
			}
			FComplexMovementRotationCommitInfo movecommit(
				roots[i].SimRoot.Get(), 
				locations[i], 
				rotations[i], 
				velocities[i], 
				bounds[i], 
				unitids[i].UnitID, 
				entities[i]);
			RotationCommitsThisFrame.Add(movecommit);
		}
	});

	RotationCommitsThisFrame.Sort();

	for (int32 i = 0; i < RotationCommitsThisFrame.Num(); i++)
	{
		RotationCommitsThisFrame[i].Rotation.Rotation += RotationCommitsThisFrame[i].Velocity.AngularVelocity;
		RotationCommitsThisFrame[i].Rotation.Rotation.Normalize();
		RotationCommitsThisFrame[i].SimRoot->SetRelativeRotation(RotationCommitsThisFrame[i].Rotation.Rotation);
	}
}
