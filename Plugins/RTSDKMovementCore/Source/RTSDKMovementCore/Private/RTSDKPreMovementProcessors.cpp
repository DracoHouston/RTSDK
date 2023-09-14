// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKPreMovementProcessors.h"
#include "RTSDKFragments.h"
#include "RTSDKMovementCoreFragments.h"
#include "RTSDKGameSimSubsystem.h"
#include "MassSignalSubsystem.h"
#include "RTSDKConstants.h"
#include "RTSDKBatchedSimCommand.h"
#include "RTSDKUnitComponent.h"

URTSDKCurrentToPrevious3DTransformProcessor::URTSDKCurrentToPrevious3DTransformProcessor()
{
	bRequiresGameThreadExecution = false;
	RTSDK_PROCESSOR_EXEC_ORDER_PRE_MOVEMENT
}

void URTSDKCurrentToPrevious3DTransformProcessor::ConfigureQueries()
{
	MovementQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSCurrentScaleFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSPreviousLocationFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSPreviousRotationFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSPreviousScaleFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSAngularVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.AddRequirement<FRTSVisRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	MovementQuery.RegisterWithProcessor(*this);
}

void URTSDKCurrentToPrevious3DTransformProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	MovementQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TConstArrayView<FRTSCurrentLocationFragment> locs = Context.GetFragmentView<FRTSCurrentLocationFragment>();
		TConstArrayView<FRTSCurrentRotationFragment> rots = Context.GetFragmentView<FRTSCurrentRotationFragment>();
		TConstArrayView<FRTSCurrentScaleFragment> scales = Context.GetFragmentView<FRTSCurrentScaleFragment>();
		TArrayView<FRTSPreviousLocationFragment> prevlocs = Context.GetMutableFragmentView<FRTSPreviousLocationFragment>();
		TArrayView<FRTSPreviousRotationFragment> prevrots = Context.GetMutableFragmentView<FRTSPreviousRotationFragment>();
		TArrayView<FRTSPreviousScaleFragment> prevscales = Context.GetMutableFragmentView<FRTSPreviousScaleFragment>();
		int32 entcount = Context.GetNumEntities();
		for (int32 i = 0; i < entcount; i++)
		{
			prevlocs[i].Location = locs[i].Location;
			prevrots[i].Rotation = rots[i].Rotation;
			prevscales[i].Scale = scales[i].Scale;
		}
	});
}
