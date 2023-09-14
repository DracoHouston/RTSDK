// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKMovementCoreTraits.h"
#include "RTSDKFragments.h"
#include "RTSDKMovementCoreFragments.h"
#include "MassCommonFragments.h"
#include "MassEntityTemplate.h"
#include "MassEntityTemplateRegistry.h"
#include "MassEntityView.h"
#include "MassEntitySubsystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "MassMovementFragments.h"
#include "Translators/MassCapsuleComponentTranslators.h"
#include "VisualLogger/VisualLogger.h"
#include "RTSDKVisRootComponent.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKGameSimSubsystem.h"


void URTSVisRootInterpolationTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.RequireFragment<FRTSUnitIDFragment>();
	BuildContext.RequireFragment<FRTSCurrentLocationFragment>();
	BuildContext.RequireFragment<FRTSCurrentRotationFragment>();
	BuildContext.RequireFragment<FRTSCurrentScaleFragment>();
	BuildContext.RequireFragment<FRTSSimRootFragment>();
	BuildContext.AddFragment<FRTSVisRootFragment>();
	BuildContext.AddFragment<FRTSPreviousLocationFragment>();
	BuildContext.AddFragment<FRTSPreviousRotationFragment>();
	BuildContext.AddFragment<FRTSPreviousScaleFragment>();
	BuildContext.GetMutableObjectFragmentInitializers().Add([=](UObject& Owner, FMassEntityView& EntityView, const EMassTranslationDirection CurrentDirection)
	{
		AActor* actorowner = Cast<AActor>(&Owner);
		if (actorowner != nullptr)
		{
			FRTSVisRootFragment& visroot = EntityView.GetFragmentData<FRTSVisRootFragment>();
			URTSDKVisRootComponent* visrootcomp = actorowner->FindComponentByClass<URTSDKVisRootComponent>();
			if (visrootcomp != nullptr)
			{
				visroot.VisRoot = visrootcomp;
			}
		}
	});
}
