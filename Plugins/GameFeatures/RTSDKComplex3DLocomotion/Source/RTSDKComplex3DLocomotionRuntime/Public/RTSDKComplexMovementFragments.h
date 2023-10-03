// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "RTSDKConstants.h"
#include "RTSDKComplexMovementFragments.generated.h"

class UPrimitiveComponent;

USTRUCT()
struct FRTSComplex3DMovementTag : public FMassTag
{
	GENERATED_BODY()

};

USTRUCT()
struct FRTSMovementComplexWalkingParamsFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSNumber64 StepUpHeight;
	FRTSNumber64 StepDownHeight;
	FRTSNumber64 MaxWalkableAngle;
};

USTRUCT()
struct FRTSPhysicsParamsFragment : public FMassFragment
{
	GENERATED_BODY()
	
	FRTSNumber64 Mass;
	FRTSNumber64 Volume;
	FRTSNumber64 Density;
};

USTRUCT()
struct FRTSMovementBasisFragment : public FMassFragment
{
	GENERATED_BODY()

	TWeakObjectPtr<UPrimitiveComponent> Basis;

	FHitResult Impact;
};
