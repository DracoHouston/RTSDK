// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKTraits.h"
#include "RTSDKFragments.h"
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



void URTSUnitRegistrationTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FRTSUnitIDFragment>();
	BuildContext.AddFragment<FRTSUnitComponentFragment>();
	BuildContext.GetMutableObjectFragmentInitializers().Add([=](UObject& Owner, FMassEntityView& EntityView, const EMassTranslationDirection CurrentDirection)
		{
			AActor* actorowner = Cast<AActor>(&Owner);
	if (actorowner != nullptr)
	{
		URTSDKUnitComponent* unit = actorowner->FindComponentByClass<URTSDKUnitComponent>();
		if (unit != nullptr)
		{
			UWorld* world = actorowner->GetWorld();
			if (world != nullptr)
			{
				URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
				if (sim != nullptr)
				{
					FRTSUnitIDFragment& unitid = EntityView.GetFragmentData<FRTSUnitIDFragment>();
					uint32 newunitid;
					if (sim->RegisterUnit(actorowner, unit, EntityView.GetEntity(), newunitid))
					{
						unitid.UnitID = newunitid;
						unit->UnitID = newunitid;
						FRTSUnitComponentFragment& unitcomp = EntityView.GetFragmentData<FRTSUnitComponentFragment>();
						unitcomp.UnitComponent = unit;
					}

				}
			}
		}
	}
		});
}
