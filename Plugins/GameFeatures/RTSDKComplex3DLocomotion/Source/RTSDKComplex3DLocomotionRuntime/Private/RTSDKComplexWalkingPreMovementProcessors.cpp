// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKComplexWalkingPreMovementProcessors.h"
#include "RTSDKFragments.h"
#include "RTSDKMovementCoreFragments.h"
#include "RTSDKComplexMovementFragments.h"
#include "RTSDKGameSimSubsystem.h"
#include "MassSignalSubsystem.h"
#include "RTSDKConstants.h"
#include "RTSDKBatchedSimCommand.h"
#include "RTSDKUnitComponent.h"

URTSDKComplexWalkingGravityChangeProcessor::URTSDKComplexWalkingGravityChangeProcessor()
{
	bRequiresGameThreadExecution = false;
	RTSDK_PROCESSOR_EXEC_ORDER_PRE_MOVEMENT
}

void URTSDKComplexWalkingGravityChangeProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	UMassSignalSubsystem* SignalSubsystem = UWorld::GetSubsystem<UMassSignalSubsystem>(Owner.GetWorld());
	SubscribeToSignal(*SignalSubsystem, UE::Mass::Signals::RTSDKUnitGravityChanged);
}

void URTSDKComplexWalkingGravityChangeProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSCurrentScaleFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSMovementInputFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSLookInputFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSMaxAccelerationFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSMaxDecelerationFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSMaxAngularAccelerationFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSMaxAngularDecelerationFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSMaxVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSCollisionBoundsFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSAngularVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	EntityQuery.AddTagRequirement<FRTSComplex3DMovementTag>(EMassFragmentPresence::All);
	EntityQuery.RegisterWithProcessor(*this);
}

void URTSDKComplexWalkingGravityChangeProcessor::SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals)
{
	URTSDKGameSimSubsystem* sim = GetWorld()->GetSubsystem<URTSDKGameSimSubsystem>();
	FRTSVector64 gravdir = sim->GetGravityDirection();
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this, gravdir](FMassExecutionContext& Context)
	{
		TArrayView<FRTSCollisionBoundsFragment> bounds = Context.GetMutableFragmentView<FRTSCollisionBoundsFragment>();
		int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			bounds[i].FeetLocation = gravdir * bounds[i].BoundsHalfHeight;
		}
	});
}
