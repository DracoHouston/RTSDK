// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "RTSDKConstants.h"
#include "RTSDKFragments.generated.h"

class URTSDKVisRootComponent;
class URTSDKUnitComponent;
class UPrimitiveComponent;

/**
*
*/
USTRUCT()
struct FRTSUnitIDFragment : public FMassFragment
{
	GENERATED_BODY()

	uint32 UnitID;
};

USTRUCT()
struct FRTSVisRootFragment : public FMassFragment
{
	GENERATED_BODY()

	TWeakObjectPtr<URTSDKVisRootComponent> VisRoot;
};

USTRUCT()
struct FRTSSimRootFragment : public FMassFragment
{
	GENERATED_BODY()

	TWeakObjectPtr<USceneComponent> SimRoot;
};

USTRUCT()
struct FRTSUnitComponentFragment : public FMassFragment
{
	GENERATED_BODY()

	TWeakObjectPtr<URTSDKUnitComponent> UnitComponent;
};
