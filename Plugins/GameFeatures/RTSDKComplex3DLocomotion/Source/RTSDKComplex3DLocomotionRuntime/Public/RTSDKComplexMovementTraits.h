// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "RTSDKTypes.h"
#include "RTSDKConstants.h"
#include "FixedPointNumbers.h"
#include "RTSDKComplexMovementTraits.generated.h"

/**
*
*/
UCLASS()
class URTSComplexWalkingMovementTrait : public UMassEntityTraitBase
{
public:
	GENERATED_BODY()

	//Maximum velocity this unit may move under their own power, in meters per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxVelocity;

	//Maximum acceleration, in meters per second squared
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 Acceleration;

	//Maximum deceleration in meters per second squared
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 Deceleration;

	//Air Control, as a scaling factor of Acceleration and Max Velocity while in the air, typically between 0.0-1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 AirControl;

	//Mass, in kilograms
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 Mass;

	//Volume, in meters cubed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 Volume;

	//Maximum height unit may step up onto walkable geometry, in meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxStepUpHeight;

	//Maximum height unit may step down onto walkable geometry, in meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxStepDownHeight;

	//Maximum walkable angle unit may walk upon, in degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxWalkableAngle;

	//Maximum angle unit may turn on the yaw axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularVelocityPitch;

	//Maximum angle unit may turn on the pitch axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularVelocityYaw;

	//Maximum angle unit may turn on the roll axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularVelocityRoll;

	//Maximum angle unit may accelerate angular velocity, on the pitch axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularAccelerationPitch;

	//Maximum angle unit may accelerate angular velocity, on the yaw axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularAccelerationYaw;

	//Maximum angle unit may accelerate angular velocity, on the roll axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularAccelerationRoll;

	//Maximum angle unit may decelerate angular velocity, on the pitch axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularDecelerationPitch;

	//Maximum angle unit may decelerate angular velocity, on the yaw axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularDecelerationYaw;

	//Maximum angle unit may decelerate angular velocity, on the roll axis, in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFixed64 MaxAngularDecelerationRoll;

protected:
	//Where the magic happens
	virtual void BuildTemplate(
		FMassEntityTemplateBuildContext& BuildContext,
		const UWorld& World) const override;
};
