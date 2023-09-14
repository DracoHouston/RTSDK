// Fill out your copyright notice in the Description page of Project Settings.


#include "DeterministicFloatQuat64.h"
#include "DeterministicFloatNumbers.h"
#include "DeterministicFloatVector64.h"
#include "DeterministicFloatVector64Funcs.h"
#include "DeterministicFloatQuat64Funcs.h"


const FDeterministicQuat64 FDeterministicQuat64::Identity = FDeterministicQuat64(FDeterministicNumber64(), FDeterministicNumber64(), FDeterministicNumber64(), FDeterministicNumber64((int64)1));

FDeterministicRotator64 FDeterministicQuat64::Rotator() const
{
	const FDeterministicNumber64 Two = FDeterministicNumber64::Make(2.0);
	const FDeterministicNumber64 Ninety = FDeterministicNumber64::Make(90.0);
	const FDeterministicNumber64 SingularityTest = Z * X - W * Y;
	const FDeterministicNumber64 YawY = Two * (W * Z + X * Y);
	const FDeterministicNumber64 YawX = (DeterministicFloat::Constants::Number64::One - Two * (FDeterministicFloatMath::Square(Y) + FDeterministicFloatMath::Square(Z)));
	
	const FDeterministicNumber64 SINGULARITY_THRESHOLD = FDeterministicNumber64::Make(DeterministicFloat::Constants::Number64::Half.Value - DeterministicFloat::Constants::Number64::Delta.Value);
	const FDeterministicNumber64 RAD_TO_DEG = DeterministicFloat::Constants::Number64::OneEightyOverPi;
	FDeterministicNumber64 Pitch, Yaw, Roll;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		Pitch = -Ninety;
		Yaw = FDeterministicFloatMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		Roll = FDeterministicRotator64::NormalizeAxis(-Yaw - (Two * FDeterministicFloatMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		Pitch = Ninety;
		Yaw = FDeterministicFloatMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		Roll = FDeterministicRotator64::NormalizeAxis(Yaw - (Two * FDeterministicFloatMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		Pitch = (FDeterministicFloatMath::Asin(Two * SingularityTest) * RAD_TO_DEG); 
		Yaw = (FDeterministicFloatMath::Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (FDeterministicFloatMath::Atan2(-Two * (W * X + Y * Z), (DeterministicFloat::Constants::Number64::One - Two * (FDeterministicFloatMath::Square(X) + FDeterministicFloatMath::Square(Y)))) * RAD_TO_DEG);
	}

	return FDeterministicRotator64(Pitch, Yaw, Roll);
}

//
	// Based on:
	// http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
	// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
	//
FORCEINLINE_DEBUGGABLE FDeterministicQuat64 FindBetween_Helper(const FDeterministicVector64& A, const FDeterministicVector64& B, FDeterministicNumber64 NormAB)
{
	FDeterministicNumber64 W = NormAB + FDeterministicVector64::DotProduct(A, B);
	FDeterministicQuat64 Result;

	if (W >= FDeterministicNumber64::Make(1e-6) * NormAB)
	{
		//Axis = FVector::CrossProduct(A, B);
		Result = FDeterministicQuat64(
			A.Y * B.Z - A.Z * B.Y,
			A.Z * B.X - A.X * B.Z,
			A.X * B.Y - A.Y * B.X,
			W);
	}
	else
	{
		// A and B point in opposite directions
		W = 0.f;
		Result = FDeterministicFloatMath::Abs(A.X) > FDeterministicFloatMath::Abs(A.Y)
			? FDeterministicQuat64(-A.Z, DeterministicFloat::Constants::Number64::Zero, A.X, W)
			: FDeterministicQuat64(DeterministicFloat::Constants::Number64::Zero, -A.Z, A.Y, W);
	}

	Result.Normalize();
	return Result;
}

FDeterministicQuat64 FDeterministicQuat64::FindBetweenNormals(const FDeterministicVector64& A, const FDeterministicVector64& B)
{
	const FDeterministicNumber64 NormAB = DeterministicFloat::Constants::Number64::One;
	return FindBetween_Helper(A, B, NormAB);
}

FDeterministicQuat64 FDeterministicQuat64::FindBetweenVectors(const FDeterministicVector64& A, const FDeterministicVector64& B)
{
	const FDeterministicNumber64 NormAB = FDeterministicFloatMath::Sqrt(A.SizeSquared() * B.SizeSquared());
	return FindBetween_Helper(A, B, NormAB);
}

FDeterministicQuat64 FDeterministicQuat64::MakeFromEuler(const FDeterministicVector64& Euler)
{
	return FDeterministicQuat64(FDeterministicRotator64::MakeFromEuler(Euler));
}

FDeterministicVector64 FDeterministicQuat64::Euler() const
{
	return Rotator().Euler();
}

FDeterministicQuat64 FDeterministicQuat64::Log() const
{
	FDeterministicQuat64 Result;
	Result.W = DeterministicFloat::Constants::Number64::Zero;

	if (FMath::Abs(W) < DeterministicFloat::Constants::Number64::One)
	{
		const FDeterministicNumber64 Angle = FDeterministicFloatMath::Acos(W);
		const FDeterministicNumber64 SinAngle = FDeterministicFloatMath::Sin(Angle);

		if (FMath::Abs(SinAngle) >= DeterministicFloat::Constants::Number64::SmallNumber)
		{
			const FDeterministicNumber64 Scale = Angle / SinAngle;
			Result.X = Scale * X;
			Result.Y = Scale * Y;
			Result.Z = Scale * Z;

			return Result;
		}
	}

	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;

	return Result;
}

FDeterministicQuat64 FDeterministicQuat64::Exp() const
{
	const FDeterministicNumber64 Angle = FDeterministicFloatMath::Sqrt(X * X + Y * Y + Z * Z);
	const FDeterministicNumber64 SinAngle = FDeterministicFloatMath::Sin(Angle);

	FDeterministicQuat64  Result;
	Result.W = FDeterministicFloatMath::Cos(Angle);

	if (FDeterministicFloatMath::Abs(SinAngle) >= DeterministicFloat::Constants::Number64::SmallNumber)
	{
		const FDeterministicNumber64 Scale = SinAngle / Angle;
		Result.X = Scale * X;
		Result.Y = Scale * Y;
		Result.Z = Scale * Z;
	}
	else
	{
		Result.X = X;
		Result.Y = Y;
		Result.Z = Z;
	}

	return Result;
}
