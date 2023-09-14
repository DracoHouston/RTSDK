// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKVisRootInterpolationTranslator.h"
#include "RTSDKFragments.h"
#include "RTSDKMovementCoreFragments.h"
#include "RTSDKTypes.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKConstants.h"
#include "RTSDKVisRootComponent.h"

URTSDKVisRootInterpolationTranslator::URTSDKVisRootInterpolationTranslator()
{
	//Run only for players, client and standalone
	ExecutionFlags = (int32)(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	bRequiresGameThreadExecution = true;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKVisInterpolation;
}

void URTSDKVisRootInterpolationTranslator::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
}

void URTSDKVisRootInterpolationTranslator::ConfigureQueries()
{
	InterpolationQuery.AddRequirement<FRTSUnitIDFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSCurrentLocationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSCurrentRotationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSCurrentScaleFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSPreviousLocationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSPreviousRotationFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSPreviousScaleFragment>(EMassFragmentAccess::ReadOnly, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSAngularVelocityFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSSimRootFragment>(EMassFragmentAccess::None, EMassFragmentPresence::All);
	InterpolationQuery.AddRequirement<FRTSVisRootFragment>(EMassFragmentAccess::ReadWrite, EMassFragmentPresence::All);
	InterpolationQuery.RegisterWithProcessor(*this);
}

void URTSDKVisRootInterpolationTranslator::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	if (sim == nullptr)
	{
		return;
	}
	if (!sim->IsSimInitialized())
	{
		return;
	}
	double alpha = FMath::Fmod(world->TimeSeconds, (double)sim->GetTimestep());
	InterpolationQuery.ForEachEntityChunk(
		EntityManager,
		Context,
		[this, alpha](FMassExecutionContext& Context)
		{
			TArrayView<FRTSVisRootFragment> visroots = Context.GetMutableFragmentView<FRTSVisRootFragment>();
			TConstArrayView<FRTSCurrentLocationFragment> locs = Context.GetMutableFragmentView<FRTSCurrentLocationFragment>();
			TConstArrayView<FRTSPreviousLocationFragment> prevlocs = Context.GetMutableFragmentView<FRTSPreviousLocationFragment>();
			TConstArrayView<FRTSCurrentRotationFragment> rots = Context.GetMutableFragmentView<FRTSCurrentRotationFragment>();
			TConstArrayView<FRTSPreviousRotationFragment> prevrots = Context.GetMutableFragmentView<FRTSPreviousRotationFragment>();
			TConstArrayView<FRTSCurrentScaleFragment> scales = Context.GetMutableFragmentView<FRTSCurrentScaleFragment>();
			TConstArrayView<FRTSPreviousScaleFragment> prevscales = Context.GetMutableFragmentView<FRTSPreviousScaleFragment>();
			int32 entcount = Context.GetNumEntities();
			for (int32 i = 0; i < entcount; i++)
			{
				FVector visloc = FMath::Lerp((FVector)locs[i].Location, (FVector)prevlocs[i].Location, alpha);
				FRotator visrot = FMath::Lerp(FRotator::ZeroRotator, (FRotator)rots[i].Rotation - (FRotator)prevrots[i].Rotation, alpha);
				FVector visscale = FMath::Lerp((FVector)scales[i].Scale, (FVector)prevscales[i].Scale, alpha);
				visroots[i].VisRoot->SetRelativeRotation(visrot);
				visroots[i].VisRoot->SetWorldScale3D(visscale);
				visroots[i].VisRoot->SetWorldLocation(visloc);
				//transforms[i].GetMutableTransform().Blend(velocities[i].PreviousTransform, velocities[i].CurrentTransform, alpha);
				//FTransform blendedtransform = FTransform::Identity;
				//FTransform currenttransform()
				//blendedtransform.Blend(velocities[i].PreviousTransform, velocities[i].CurrentTransform, alpha);
				//visroots[i].VisRoot.Get()->SetWorldTransform(blendedtransform);
			}
		}
	);	
}
