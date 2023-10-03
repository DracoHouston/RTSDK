// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKComplexWalkingVelocityDirectors.h"
#include "RTSDKFragments.h"
#include "RTSDKMovementCoreFragments.h"
#include "RTSDKComplexMovementFragments.h"
#include "RTSDKGameSimSubsystem.h"
#include "MassSignalSubsystem.h"
#include "RTSDKConstants.h"

URTSDKComplexWalkingBasedVelocityDirector::URTSDKComplexWalkingBasedVelocityDirector()
{
	bRequiresGameThreadExecution = false;
	RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_VELOCITY_DIRECTORS
}

URTSDKComplexWalkingAirVelocityDirector::URTSDKComplexWalkingAirVelocityDirector()
{
	bRequiresGameThreadExecution = false;
	RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_VELOCITY_DIRECTORS
}

URTSDKComplexWalkingAngularVelocityDirector::URTSDKComplexWalkingAngularVelocityDirector()
{
	bRequiresGameThreadExecution = false;
	RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_VELOCITY_DIRECTORS
}

void URTSDKComplexWalkingBasedVelocityDirector::ConfigureQueries()
{
	MovementQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMovementInputFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxAccelerationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxDecelerationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxVelocityFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMovementBasisFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);
	MovementQuery.AddTagRequirement<FRTSWalkingMovementTag>(EMassFragmentPresence::All);

	MovementQuery.RegisterWithProcessor(*this);
}

void URTSDKComplexWalkingAirVelocityDirector::ConfigureQueries()
{
	MovementQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMovementInputFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxAccelerationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxDecelerationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxVelocityFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMovementBasisFragment>(EMassFragmentAccess::None, EMassFragmentPresence::None);
	MovementQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSAirControlFactorFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);
	MovementQuery.AddTagRequirement<FRTSWalkingMovementTag>(EMassFragmentPresence::All);

	MovementQuery.RegisterWithProcessor(*this);
}

void URTSDKComplexWalkingAngularVelocityDirector::ConfigureQueries()
{
	MovementQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSLookInputFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxAngularAccelerationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxAngularDecelerationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSMaxAngularVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSAngularVelocityFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);
	MovementQuery.AddTagRequirement<FRTSWalkingMovementTag>(EMassFragmentPresence::All);
	
	MovementQuery.RegisterWithProcessor(*this);
}

void URTSDKComplexWalkingBasedVelocityDirector::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	TArray<FMassEntityHandle> movingentities = TArray<FMassEntityHandle>();
	movingentities.Empty(16);
	MovementQuery.ForEachEntityChunk(EntityManager,	Context, 
		[this, &movingentities](FMassExecutionContext& Context)
	{
		TConstArrayView <FMassEntityHandle> entities = Context.GetEntities();
		TConstArrayView<FRTSMaxAccelerationFragment> accelerations = Context.GetFragmentView<FRTSMaxAccelerationFragment>();
		TConstArrayView<FRTSMaxDecelerationFragment> decelerations = Context.GetFragmentView<FRTSMaxDecelerationFragment>();
		TConstArrayView<FRTSMaxVelocityFragment> maxvelocities = Context.GetFragmentView<FRTSMaxVelocityFragment>();
		TConstArrayView<FRTSMovementInputFragment> inputs = Context.GetFragmentView<FRTSMovementInputFragment>();
		TArrayView<FRTSMovementBasisFragment> bases = Context.GetMutableFragmentView<FRTSMovementBasisFragment>();
		TArrayView<FRTSVelocityFragment> velocities = Context.GetMutableFragmentView<FRTSVelocityFragment>();
		int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			velocities[i].Velocity += FRTSVector64::VectorPlaneProject(inputs[i].Input * accelerations[i].Acceleration, bases[i].Impact.ImpactNormal);
			velocities[i].Velocity = velocities[i].Velocity.SizeSquared() > FRTSMath::Square(maxvelocities[i].Velocity) ? 
				velocities[i].Velocity.GetSafeNormal() * maxvelocities[i].Velocity : 
				velocities[i].Velocity;
			if (!velocities[i].Velocity.IsNearlyZero())
			{
				movingentities.Add(entities[i]);
			}
		}
	});
	if (movingentities.Num() > 0)
	{
		MassSignalSubsystem->SignalEntities(UE::Mass::Signals::RTSDKUnitHasVelocity, movingentities);
	}
}

void URTSDKComplexWalkingAirVelocityDirector::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	const FRTSVector64 grav = SimSubsystem->GetGravityVector();
	const FRTSNumber64 terminalvelocity = SimSubsystem->GetTerminalVelocity();
	const FRTSNumber64 terminalvelocitysquared = FRTSMath::Square(terminalvelocity);
	TArray<FMassEntityHandle> movingentities = TArray<FMassEntityHandle>();
	movingentities.Empty(16);
	MovementQuery.ForEachEntityChunk(EntityManager,	Context, 
		[this, grav, terminalvelocity, terminalvelocitysquared, &movingentities](FMassExecutionContext& Context)
	{
		TConstArrayView<FMassEntityHandle> entities = Context.GetEntities();
		TConstArrayView<FRTSMaxAccelerationFragment> accelerations = Context.GetFragmentView<FRTSMaxAccelerationFragment>();
		TConstArrayView<FRTSMaxDecelerationFragment> decelerations = Context.GetFragmentView<FRTSMaxDecelerationFragment>();
		TConstArrayView<FRTSMaxVelocityFragment> maxvelocities = Context.GetFragmentView<FRTSMaxVelocityFragment>();
		TConstArrayView<FRTSAirControlFactorFragment> aircontrol = Context.GetFragmentView<FRTSAirControlFactorFragment>();
		TConstArrayView<FRTSMovementInputFragment> inputs = Context.GetFragmentView<FRTSMovementInputFragment>();
		TArrayView<FRTSVelocityFragment> velocities = Context.GetMutableFragmentView<FRTSVelocityFragment>();

		const int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			FRTSVector64 acceleration = inputs[i].Input * (accelerations[i].Acceleration * aircontrol[i].AirControl);
			FRTSVector64 testvelocity = velocities[i].Velocity + acceleration;
			velocities[i].Velocity = testvelocity.SizeSquared2D() > FRTSMath::Square(maxvelocities[i].Velocity * aircontrol[i].AirControl) ? velocities[i].Velocity : testvelocity;
			velocities[i].Velocity += grav;
			velocities[i].Velocity = velocities[i].Velocity.SizeSquared() > terminalvelocitysquared ? velocities[i].Velocity.GetSafeNormal() * terminalvelocity : velocities[i].Velocity;
			if (!velocities[i].Velocity.IsNearlyZero())
			{
				movingentities.Add(entities[i]);
			}
		}
	});
	if (movingentities.Num() > 0)
	{
		MassSignalSubsystem->SignalEntities(UE::Mass::Signals::RTSDKUnitHasVelocity, movingentities);
	}
}

void URTSDKComplexWalkingAngularVelocityDirector::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	FRTSRotator64 gravangle = SimSubsystem->GetGravityDirection().ToOrientationRotator();
	gravangle.Pitch += FRTSNumber64::Make(90.0);
	TArray<FMassEntityHandle> movingentities;
	movingentities.Empty(16);
	MovementQuery.ForEachEntityChunk(EntityManager,	Context, 
		[this, gravangle, &movingentities](FMassExecutionContext& Context)
	{
		TConstArrayView<FMassEntityHandle> entities = Context.GetEntities();
		TConstArrayView<FRTSMaxAngularAccelerationFragment> accelerations = Context.GetFragmentView<FRTSMaxAngularAccelerationFragment>();
		TConstArrayView<FRTSMaxAngularDecelerationFragment> decelerations = Context.GetFragmentView<FRTSMaxAngularDecelerationFragment>();
		TConstArrayView<FRTSMaxAngularVelocityFragment> maxvelocities = Context.GetFragmentView<FRTSMaxAngularVelocityFragment>();
		TConstArrayView<FRTSLookInputFragment> lookinputs = Context.GetFragmentView<FRTSLookInputFragment>();
		TConstArrayView<FRTSCurrentRotationFragment> rots = Context.GetFragmentView<FRTSCurrentRotationFragment>();
		TArrayView<FRTSAngularVelocityFragment> velocities = Context.GetMutableFragmentView<FRTSAngularVelocityFragment>();

		const int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			FRTSRotator64 desiredrot = FRTSRotator64(gravangle.Pitch, lookinputs[i].Input.Yaw, gravangle.Yaw);

			FRTSNumber64 testpitch = FRTSMath::Abs(velocities[i].AngularVelocity.Pitch) - FRTSMath::Abs(accelerations[i].AngularAcceleration.Pitch);
			FRTSNumber64 testyaw = FRTSMath::Abs(velocities[i].AngularVelocity.Yaw) - FRTSMath::Abs(accelerations[i].AngularAcceleration.Yaw);
			FRTSNumber64 testroll = FRTSMath::Abs(velocities[i].AngularVelocity.Roll) - FRTSMath::Abs(accelerations[i].AngularAcceleration.Roll);
			//if less than or equal 0 (we can stop our current angular velocity) we apply it to get a new one towards goal
			//if greater than 0 we apply our angular acceleration to current velocity towards 0
			FRTSNumber64 deltapitch = FRTSMath::FindDeltaAngleDegrees(rots[i].Rotation.Pitch, desiredrot.Pitch);
			FRTSNumber64 deltayaw = FRTSMath::FindDeltaAngleDegrees(rots[i].Rotation.Yaw, desiredrot.Yaw);
			FRTSNumber64 deltaroll = FRTSMath::FindDeltaAngleDegrees(rots[i].Rotation.Roll, desiredrot.Roll);
			testpitch = testpitch <= FRTSNumber64::Make(0.0) ?
				FRTSMath::Min(FRTSMath::Abs(deltapitch), accelerations[i].AngularAcceleration.Pitch) * FRTSMath::Sign(deltapitch) :
				FRTSMath::FixedTurn(velocities[i].AngularVelocity.Pitch, FRTSNumber64::Make(0.0), accelerations[i].AngularAcceleration.Pitch);
			testyaw = testyaw <= FRTSNumber64::Make(0.0) ?
				FRTSMath::Min(FRTSMath::Abs(deltayaw), accelerations[i].AngularAcceleration.Yaw) * FRTSMath::Sign(deltayaw) :
				FRTSMath::FixedTurn(velocities[i].AngularVelocity.Yaw, FRTSNumber64::Make(0.0), accelerations[i].AngularAcceleration.Yaw);
			testroll = testroll <= FRTSNumber64::Make(0.0) ?
				FRTSMath::Min(FRTSMath::Abs(deltaroll), accelerations[i].AngularAcceleration.Roll) * FRTSMath::Sign(deltaroll) :
				FRTSMath::FixedTurn(velocities[i].AngularVelocity.Roll, FRTSNumber64::Make(0.0), accelerations[i].AngularAcceleration.Roll);

			velocities[i].AngularVelocity = FRTSRotator64(testpitch, testyaw, testroll);
			if (!velocities[i].AngularVelocity.IsNearlyZero())
			{
				movingentities.Add(entities[i]);
			}
		}
	});
	if (movingentities.Num() > 0)
	{
		MassSignalSubsystem->SignalEntities(UE::Mass::Signals::RTSDKUnitHasAngularVelocity, movingentities);
	}
}
