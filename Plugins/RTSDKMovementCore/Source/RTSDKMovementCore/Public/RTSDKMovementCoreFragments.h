// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "RTSDKConstants.h"
#include "RTSDKMovementCoreFragments.generated.h"

USTRUCT()
struct FRTSWalkingMovementTag : public FMassTag
{
	GENERATED_BODY()

};

USTRUCT()
struct FRTSCurrentLocationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 Location;
};

USTRUCT()
struct FRTSPreviousLocationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 Location;
};

USTRUCT()
struct FRTSCurrentScaleFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 Scale;
};

USTRUCT()
struct FRTSPreviousScaleFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 Scale;
};

USTRUCT()
struct FRTSCurrentRotationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 Rotation;
};

USTRUCT()
struct FRTSPreviousRotationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 Rotation;
};

USTRUCT()
struct FRTSVelocityFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 Velocity;
};

USTRUCT()
struct FRTSAngularVelocityFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 AngularVelocity;

};

USTRUCT()
struct FRTSMovementInputFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 Input;
};

USTRUCT()
struct FRTSLookInputFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 Input;
};

//Factor to scale Max Velocity while in the air. Usually 0.0-1.0 but can go higher for increased speed in the air.
USTRUCT()
struct FRTSAirControlFactorFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSNumber64 AirControl;
};

//Fastest this unit may move under their own power, in UU per frame
USTRUCT()
struct FRTSMaxVelocityFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSNumber64 Velocity;
};

//Fastest this unit may turn under their own power, in degrees per frame
USTRUCT()
struct FRTSMaxAngularVelocityFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 AngularVelocity;
};

//Maximum amount this unit may increase velocity by under their own power, in UU per frame
USTRUCT()
struct FRTSMaxAccelerationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSNumber64 Acceleration;
};

//Maximum amount this unit may increase angular velocity by under their own power, in degrees per frame
USTRUCT()
struct FRTSMaxAngularAccelerationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 AngularAcceleration;
};

//Maximum amount this unit may decrease velocity by under their own power, in UU per frame
USTRUCT()
struct FRTSMaxDecelerationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSNumber64 Deceleration;
};

//Maximum amount this unit may decrease angular velocity by under their own power, in degrees per frame
USTRUCT()
struct FRTSMaxAngularDecelerationFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSRotator64 AngularDeceleration;
};

USTRUCT()
struct FRTSCollisionBoundsFragment : public FMassFragment
{
	GENERATED_BODY()

	FRTSVector64 BoundsSize;
	FRTSVector64 BoundsMin;
	FRTSVector64 BoundsMax;
	FRTSNumber64 BoundsHalfHeight;
	FRTSNumber64 BoundsRadius;
	FRTSVector64 FeetLocation;
};
