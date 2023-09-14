// Copyright Epic Games, Inc. All Rights Reserved.

#include "LockstepTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "FixedPointNumbers.h"
#include "Misc/FileHelper.h"

ALockstepTestCharacter::ALockstepTestCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ALockstepTestCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ALockstepTestCharacter::OnRep_fixedpointtest()
{
	OnFixedPointTestChanged();
}

void ALockstepTestCharacter::DumpConstantsAsFixed()
{
	FFixed64 FP_PI(UE_PI);
	FFixed64 FP_SMALL_NUMBER(UE_SMALL_NUMBER);
	FFixed64 FP_KINDA_SMALL_NUMBER(UE_KINDA_SMALL_NUMBER);
	FFixed64 FP_BIG_NUMBER(UE_BIG_NUMBER);
	FFixed64 FP_EULERS_NUMBER(UE_EULERS_NUMBER);
	FFixed64 FP_GOLDEN_RATIO(UE_GOLDEN_RATIO);
	FFixed64 FP_FLOAT_NON_FRACTIONAL(UE_FLOAT_NON_FRACTIONAL);
	FFixed64 FP_DOUBLE_PI(UE_DOUBLE_PI);
	FFixed64 FP_DOUBLE_SMALL_NUMBER(UE_DOUBLE_SMALL_NUMBER);
	FFixed64 FP_DOUBLE_KINDA_SMALL_NUMBER(UE_DOUBLE_KINDA_SMALL_NUMBER);
	FFixed64 FP_DOUBLE_BIG_NUMBER(UE_DOUBLE_BIG_NUMBER);
	FFixed64 FP_DOUBLE_EULERS_NUMBER(UE_DOUBLE_EULERS_NUMBER);
	FFixed64 FP_DOUBLE_GOLDEN_RATIO(UE_DOUBLE_GOLDEN_RATIO);
	FFixed64 FP_DOUBLE_NON_FRACTIONAL(UE_DOUBLE_NON_FRACTIONAL);
	FFixed64 FP_MAX_FLT(UE_MAX_FLT);
	FFixed64 FP_INV_PI(UE_INV_PI);
	FFixed64 FP_HALF_PI(UE_HALF_PI);
	FFixed64 FP_TWO_PI(UE_TWO_PI);
	FFixed64 FP_PI_SQUARED(UE_PI_SQUARED);
	FFixed64 FP_DOUBLE_INV_PI(UE_DOUBLE_INV_PI);
	FFixed64 FP_DOUBLE_HALF_PI(UE_DOUBLE_HALF_PI);
	FFixed64 FP_DOUBLE_TWO_PI(UE_DOUBLE_TWO_PI);
	FFixed64 FP_DOUBLE_PI_SQUARED(UE_DOUBLE_PI_SQUARED);
	FFixed64 FP_SQRT_2(UE_SQRT_2);
	FFixed64 FP_SQRT_3(UE_SQRT_3);
	FFixed64 FP_INV_SQRT_2(UE_INV_SQRT_2);
	FFixed64 FP_INV_SQRT_3(UE_INV_SQRT_3);
	FFixed64 FP_HALF_SQRT_2(UE_HALF_SQRT_2);
	FFixed64 FP_HALF_SQRT_3(UE_HALF_SQRT_3);
	FFixed64 FP_DOUBLE_SQRT_2(UE_DOUBLE_SQRT_2);
	FFixed64 FP_DOUBLE_SQRT_3(UE_DOUBLE_SQRT_3);
	FFixed64 FP_DOUBLE_INV_SQRT_2(UE_DOUBLE_INV_SQRT_2);
	FFixed64 FP_DOUBLE_INV_SQRT_3(UE_DOUBLE_INV_SQRT_3);
	FFixed64 FP_DOUBLE_HALF_SQRT_2(UE_DOUBLE_HALF_SQRT_2);
	FFixed64 FP_DOUBLE_HALF_SQRT_3(UE_DOUBLE_HALF_SQRT_3);
	FFixed64 FP_DELTA(UE_DELTA);
	FFixed64 FP_DOUBLE_DELTA(UE_DOUBLE_DELTA);
	FFixed64 FP_FLOAT_NORMAL_THRESH(UE_FLOAT_NORMAL_THRESH);
	FFixed64 FP_DOUBLE_NORMAL_THRESH(UE_DOUBLE_NORMAL_THRESH);
	FFixed64 FP_THRESH_POINT_ON_PLANE(UE_THRESH_POINT_ON_PLANE);
	FFixed64 FP_THRESH_POINT_ON_SIDE(UE_THRESH_POINT_ON_SIDE);
	FFixed64 FP_THRESH_POINTS_ARE_SAME(UE_THRESH_POINTS_ARE_SAME);
	FFixed64 FP_THRESH_POINTS_ARE_NEAR(UE_THRESH_POINTS_ARE_NEAR);
	FFixed64 FP_THRESH_NORMALS_ARE_SAME(UE_THRESH_NORMALS_ARE_SAME);
	FFixed64 FP_THRESH_UVS_ARE_SAME(UE_THRESH_UVS_ARE_SAME);
	FFixed64 FP_THRESH_VECTORS_ARE_NEAR(UE_THRESH_VECTORS_ARE_NEAR);
	FFixed64 FP_THRESH_SPLIT_POLY_WITH_PLANE(UE_THRESH_SPLIT_POLY_WITH_PLANE);
	FFixed64 FP_THRESH_SPLIT_POLY_PRECISELY(UE_THRESH_SPLIT_POLY_PRECISELY);
	FFixed64 FP_THRESH_ZERO_NORM_SQUARED(UE_THRESH_ZERO_NORM_SQUARED);
	FFixed64 FP_THRESH_NORMALS_ARE_PARALLEL(UE_THRESH_NORMALS_ARE_PARALLEL);
	FFixed64 FP_THRESH_NORMALS_ARE_ORTHOGONAL(UE_THRESH_NORMALS_ARE_ORTHOGONAL);
	FFixed64 FP_THRESH_VECTOR_NORMALIZED(UE_THRESH_VECTOR_NORMALIZED);
	FFixed64 FP_THRESH_QUAT_NORMALIZED(UE_THRESH_QUAT_NORMALIZED);
	FFixed64 FP_DOUBLE_THRESH_POINT_ON_PLANE(UE_DOUBLE_THRESH_POINT_ON_PLANE);
	FFixed64 FP_DOUBLE_THRESH_POINT_ON_SIDE(UE_DOUBLE_THRESH_POINT_ON_SIDE);
	FFixed64 FP_DOUBLE_THRESH_POINTS_ARE_SAME(UE_DOUBLE_THRESH_POINTS_ARE_SAME);
	FFixed64 FP_DOUBLE_THRESH_POINTS_ARE_NEAR(UE_DOUBLE_THRESH_POINTS_ARE_NEAR);
	FFixed64 FP_DOUBLE_THRESH_NORMALS_ARE_SAME(UE_DOUBLE_THRESH_NORMALS_ARE_SAME);
	FFixed64 FP_DOUBLE_THRESH_UVS_ARE_SAME(UE_DOUBLE_THRESH_UVS_ARE_SAME);
	FFixed64 FP_DOUBLE_THRESH_VECTORS_ARE_NEAR(UE_DOUBLE_THRESH_VECTORS_ARE_NEAR);
	FFixed64 FP_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE(UE_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE);
	FFixed64 FP_DOUBLE_THRESH_SPLIT_POLY_PRECISELY(UE_DOUBLE_THRESH_SPLIT_POLY_PRECISELY);
	FFixed64 FP_DOUBLE_THRESH_ZERO_NORM_SQUARED(UE_DOUBLE_THRESH_ZERO_NORM_SQUARED);
	FFixed64 FP_DOUBLE_THRESH_NORMALS_ARE_PARALLEL(UE_DOUBLE_THRESH_NORMALS_ARE_PARALLEL);
	FFixed64 FP_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL(UE_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL);
	FFixed64 FP_DOUBLE_THRESH_VECTOR_NORMALIZED(UE_DOUBLE_THRESH_VECTOR_NORMALIZED);
	FFixed64 FP_DOUBLE_THRESH_QUAT_NORMALIZED(UE_DOUBLE_THRESH_QUAT_NORMALIZED);
	FFixed64 FP_ZERO_ANIMWEIGHT_THRESH(ZERO_ANIMWEIGHT_THRESH);

	FFixed64 FP_ACOSMAGICONE((double)-0.0187293);
	FFixed64 FP_ACOSMAGICTWO((double)0.0742610);
	FFixed64 FP_ACOSMAGICTHREE((double)0.2121144);

	FFixed64 FP_ATANMAGICONE((double)+7.2128853633444123e-03f);
	FFixed64 FP_ATANMAGICTWO((double)-3.5059680836411644e-02f);
	FFixed64 FP_ATANMAGICTHREE((double)+8.1675882859940430e-02f);
	FFixed64 FP_ATANMAGICFOUR((double)-1.3374657325451267e-01f);
	FFixed64 FP_ATANMAGICFIVE((double)+1.9856563505717162e-01f);
	FFixed64 FP_ATANMAGICSIX((double)-3.3324998579202170e-01f);

	TArray<FString> lines;
	lines.Add(FString::Printf(TEXT("FIXED 64")));
	lines.Add(FString::Printf(TEXT("UE_PI:										%i"), FP_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_SMALL_NUMBER:							%i"), FP_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_KINDA_SMALL_NUMBER:						%i"), FP_KINDA_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_BIG_NUMBER:								%i"), FP_BIG_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_EULERS_NUMBER:							%i"), FP_EULERS_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_GOLDEN_RATIO:							%i"), FP_GOLDEN_RATIO.Value));
	lines.Add(FString::Printf(TEXT("UE_FLOAT_NON_FRACTIONAL:					%i"), FP_FLOAT_NON_FRACTIONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_PI:								%i"), FP_DOUBLE_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_SMALL_NUMBER:						%i"), FP_DOUBLE_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_KINDA_SMALL_NUMBER:				%i"), FP_DOUBLE_KINDA_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_BIG_NUMBER:						%i"), FP_DOUBLE_BIG_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_EULERS_NUMBER:					%i"), FP_DOUBLE_EULERS_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_GOLDEN_RATIO:						%i"), FP_DOUBLE_GOLDEN_RATIO.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_NON_FRACTIONAL:					%i"), FP_DOUBLE_NON_FRACTIONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_MAX_FLT:									%i"), FP_MAX_FLT.Value));
	lines.Add(FString::Printf(TEXT("UE_INV_PI:									%i"), FP_INV_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_HALF_PI:									%i"), FP_HALF_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_TWO_PI:									%i"), FP_TWO_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_PI_SQUARED:								%i"), FP_PI_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_INV_PI:							%i"), FP_DOUBLE_INV_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_HALF_PI:							%i"), FP_DOUBLE_HALF_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_TWO_PI:							%i"), FP_DOUBLE_TWO_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_PI_SQUARED:						%i"), FP_DOUBLE_PI_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_SQRT_2:									%i"), FP_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_SQRT_3:									%i"), FP_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_INV_SQRT_2:								%i"), FP_INV_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_INV_SQRT_3:								%i"), FP_INV_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_HALF_SQRT_2:								%i"), FP_HALF_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_HALF_SQRT_3:								%i"), FP_HALF_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_SQRT_2:							%i"), FP_DOUBLE_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_SQRT_3:							%i"), FP_DOUBLE_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_INV_SQRT_2:						%i"), FP_DOUBLE_INV_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_INV_SQRT_3:						%i"), FP_DOUBLE_INV_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_HALF_SQRT_2:						%i"), FP_DOUBLE_HALF_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_HALF_SQRT_3:						%i"), FP_DOUBLE_HALF_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DELTA:									%i"), FP_DELTA.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_DELTA:							%i"), FP_DOUBLE_DELTA.Value));
	lines.Add(FString::Printf(TEXT("UE_FLOAT_NORMAL_THRESH:						%i"), FP_FLOAT_NORMAL_THRESH.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_NORMAL_THRESH:					%i"), FP_DOUBLE_NORMAL_THRESH.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINT_ON_PLANE:					%i"), FP_THRESH_POINT_ON_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINT_ON_SIDE:					%i"), FP_THRESH_POINT_ON_SIDE.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINTS_ARE_SAME:					%i"), FP_THRESH_POINTS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINTS_ARE_NEAR:					%i"), FP_THRESH_POINTS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_NORMALS_ARE_SAME:					%i"), FP_THRESH_NORMALS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_UVS_ARE_SAME:						%i"), FP_THRESH_UVS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_VECTORS_ARE_NEAR:					%i"), FP_THRESH_VECTORS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_SPLIT_POLY_WITH_PLANE:			%i"), FP_THRESH_SPLIT_POLY_WITH_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_SPLIT_POLY_PRECISELY:				%i"), FP_THRESH_SPLIT_POLY_PRECISELY.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_ZERO_NORM_SQUARED:				%i"), FP_THRESH_ZERO_NORM_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_NORMALS_ARE_PARALLEL:				%i"), FP_THRESH_NORMALS_ARE_PARALLEL.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_NORMALS_ARE_ORTHOGONAL:			%i"), FP_THRESH_NORMALS_ARE_ORTHOGONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_VECTOR_NORMALIZED:				%i"), FP_THRESH_VECTOR_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_QUAT_NORMALIZED:					%i"), FP_THRESH_QUAT_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINT_ON_PLANE:			%i"), FP_DOUBLE_THRESH_POINT_ON_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINT_ON_SIDE:				%i"), FP_DOUBLE_THRESH_POINT_ON_SIDE.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINTS_ARE_SAME:			%i"), FP_DOUBLE_THRESH_POINTS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINTS_ARE_NEAR:			%i"), FP_DOUBLE_THRESH_POINTS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_NORMALS_ARE_SAME:			%i"), FP_DOUBLE_THRESH_NORMALS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_UVS_ARE_SAME:				%i"), FP_DOUBLE_THRESH_UVS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_VECTORS_ARE_NEAR:			%i"), FP_DOUBLE_THRESH_VECTORS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE:		%i"), FP_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_SPLIT_POLY_PRECISELY:		%i"), FP_DOUBLE_THRESH_SPLIT_POLY_PRECISELY.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_ZERO_NORM_SQUARED:			%i"), FP_DOUBLE_THRESH_ZERO_NORM_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_NORMALS_ARE_PARALLEL:		%i"), FP_DOUBLE_THRESH_NORMALS_ARE_PARALLEL.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL:	%i"), FP_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_VECTOR_NORMALIZED:			%i"), FP_DOUBLE_THRESH_VECTOR_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_QUAT_NORMALIZED:			%i"), FP_DOUBLE_THRESH_QUAT_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("ZERO_ANIMWEIGHT_THRESH:						%i"), FP_ZERO_ANIMWEIGHT_THRESH.Value));
	lines.Add(FString::Printf(TEXT("ACOSMAGICONE:								%i"), FP_ACOSMAGICONE.Value));
	lines.Add(FString::Printf(TEXT("ACOSMAGICTWO:								%i"), FP_ACOSMAGICTWO.Value));
	lines.Add(FString::Printf(TEXT("ACOSMAGICTHREE:								%i"), FP_ACOSMAGICTHREE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICONE:								%i"), FP_ATANMAGICONE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICTWO:								%i"), FP_ATANMAGICTWO.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICTHREE:								%i"), FP_ATANMAGICTHREE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICFOUR:								%i"), FP_ATANMAGICFOUR.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICFIVE:								%i"), FP_ATANMAGICFIVE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICSIX:								%i"), FP_ATANMAGICSIX.Value));
	FFixed32 SP_PI(UE_PI);
	FFixed32 SP_SMALL_NUMBER(UE_SMALL_NUMBER);
	FFixed32 SP_KINDA_SMALL_NUMBER(UE_KINDA_SMALL_NUMBER);
	FFixed32 SP_BIG_NUMBER(UE_BIG_NUMBER);
	FFixed32 SP_EULERS_NUMBER(UE_EULERS_NUMBER);
	FFixed32 SP_GOLDEN_RATIO(UE_GOLDEN_RATIO);
	FFixed32 SP_FLOAT_NON_FRACTIONAL(UE_FLOAT_NON_FRACTIONAL);
	FFixed32 SP_DOUBLE_PI(UE_DOUBLE_PI);
	FFixed32 SP_DOUBLE_SMALL_NUMBER(UE_DOUBLE_SMALL_NUMBER);
	FFixed32 SP_DOUBLE_KINDA_SMALL_NUMBER(UE_DOUBLE_KINDA_SMALL_NUMBER);
	FFixed32 SP_DOUBLE_BIG_NUMBER(UE_DOUBLE_BIG_NUMBER);
	FFixed32 SP_DOUBLE_EULERS_NUMBER(UE_DOUBLE_EULERS_NUMBER);
	FFixed32 SP_DOUBLE_GOLDEN_RATIO(UE_DOUBLE_GOLDEN_RATIO);
	FFixed32 SP_DOUBLE_NON_FRACTIONAL(UE_DOUBLE_NON_FRACTIONAL);
	FFixed32 SP_MAX_FLT(UE_MAX_FLT);
	FFixed32 SP_INV_PI(UE_INV_PI);
	FFixed32 SP_HALF_PI(UE_HALF_PI);
	FFixed32 SP_TWO_PI(UE_TWO_PI);
	FFixed32 SP_PI_SQUARED(UE_PI_SQUARED);
	FFixed32 SP_DOUBLE_INV_PI(UE_DOUBLE_INV_PI);
	FFixed32 SP_DOUBLE_HALF_PI(UE_DOUBLE_HALF_PI);
	FFixed32 SP_DOUBLE_TWO_PI(UE_DOUBLE_TWO_PI);
	FFixed32 SP_DOUBLE_PI_SQUARED(UE_DOUBLE_PI_SQUARED);
	FFixed32 SP_SQRT_2(UE_SQRT_2);
	FFixed32 SP_SQRT_3(UE_SQRT_3);
	FFixed32 SP_INV_SQRT_2(UE_INV_SQRT_2);
	FFixed32 SP_INV_SQRT_3(UE_INV_SQRT_3);
	FFixed32 SP_HALF_SQRT_2(UE_HALF_SQRT_2);
	FFixed32 SP_HALF_SQRT_3(UE_HALF_SQRT_3);
	FFixed32 SP_DOUBLE_SQRT_2(UE_DOUBLE_SQRT_2);
	FFixed32 SP_DOUBLE_SQRT_3(UE_DOUBLE_SQRT_3);
	FFixed32 SP_DOUBLE_INV_SQRT_2(UE_DOUBLE_INV_SQRT_2);
	FFixed32 SP_DOUBLE_INV_SQRT_3(UE_DOUBLE_INV_SQRT_3);
	FFixed32 SP_DOUBLE_HALF_SQRT_2(UE_DOUBLE_HALF_SQRT_2);
	FFixed32 SP_DOUBLE_HALF_SQRT_3(UE_DOUBLE_HALF_SQRT_3);
	FFixed32 SP_DELTA(UE_DELTA);
	FFixed32 SP_DOUBLE_DELTA(UE_DOUBLE_DELTA);
	FFixed32 SP_FLOAT_NORMAL_THRESH(UE_FLOAT_NORMAL_THRESH);
	FFixed32 SP_DOUBLE_NORMAL_THRESH(UE_DOUBLE_NORMAL_THRESH);
	FFixed32 SP_THRESH_POINT_ON_PLANE(UE_THRESH_POINT_ON_PLANE);
	FFixed32 SP_THRESH_POINT_ON_SIDE(UE_THRESH_POINT_ON_SIDE);
	FFixed32 SP_THRESH_POINTS_ARE_SAME(UE_THRESH_POINTS_ARE_SAME);
	FFixed32 SP_THRESH_POINTS_ARE_NEAR(UE_THRESH_POINTS_ARE_NEAR);
	FFixed32 SP_THRESH_NORMALS_ARE_SAME(UE_THRESH_NORMALS_ARE_SAME);
	FFixed32 SP_THRESH_UVS_ARE_SAME(UE_THRESH_UVS_ARE_SAME);
	FFixed32 SP_THRESH_VECTORS_ARE_NEAR(UE_THRESH_VECTORS_ARE_NEAR);
	FFixed32 SP_THRESH_SPLIT_POLY_WITH_PLANE(UE_THRESH_SPLIT_POLY_WITH_PLANE);
	FFixed32 SP_THRESH_SPLIT_POLY_PRECISELY(UE_THRESH_SPLIT_POLY_PRECISELY);
	FFixed32 SP_THRESH_ZERO_NORM_SQUARED(UE_THRESH_ZERO_NORM_SQUARED);
	FFixed32 SP_THRESH_NORMALS_ARE_PARALLEL(UE_THRESH_NORMALS_ARE_PARALLEL);
	FFixed32 SP_THRESH_NORMALS_ARE_ORTHOGONAL(UE_THRESH_NORMALS_ARE_ORTHOGONAL);
	FFixed32 SP_THRESH_VECTOR_NORMALIZED(UE_THRESH_VECTOR_NORMALIZED);
	FFixed32 SP_THRESH_QUAT_NORMALIZED(UE_THRESH_QUAT_NORMALIZED);
	FFixed32 SP_DOUBLE_THRESH_POINT_ON_PLANE(UE_DOUBLE_THRESH_POINT_ON_PLANE);
	FFixed32 SP_DOUBLE_THRESH_POINT_ON_SIDE(UE_DOUBLE_THRESH_POINT_ON_SIDE);
	FFixed32 SP_DOUBLE_THRESH_POINTS_ARE_SAME(UE_DOUBLE_THRESH_POINTS_ARE_SAME);
	FFixed32 SP_DOUBLE_THRESH_POINTS_ARE_NEAR(UE_DOUBLE_THRESH_POINTS_ARE_NEAR);
	FFixed32 SP_DOUBLE_THRESH_NORMALS_ARE_SAME(UE_DOUBLE_THRESH_NORMALS_ARE_SAME);
	FFixed32 SP_DOUBLE_THRESH_UVS_ARE_SAME(UE_DOUBLE_THRESH_UVS_ARE_SAME);
	FFixed32 SP_DOUBLE_THRESH_VECTORS_ARE_NEAR(UE_DOUBLE_THRESH_VECTORS_ARE_NEAR);
	FFixed32 SP_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE(UE_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE);
	FFixed32 SP_DOUBLE_THRESH_SPLIT_POLY_PRECISELY(UE_DOUBLE_THRESH_SPLIT_POLY_PRECISELY);
	FFixed32 SP_DOUBLE_THRESH_ZERO_NORM_SQUARED(UE_DOUBLE_THRESH_ZERO_NORM_SQUARED);
	FFixed32 SP_DOUBLE_THRESH_NORMALS_ARE_PARALLEL(UE_DOUBLE_THRESH_NORMALS_ARE_PARALLEL);
	FFixed32 SP_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL(UE_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL);
	FFixed32 SP_DOUBLE_THRESH_VECTOR_NORMALIZED(UE_DOUBLE_THRESH_VECTOR_NORMALIZED);
	FFixed32 SP_DOUBLE_THRESH_QUAT_NORMALIZED(UE_DOUBLE_THRESH_QUAT_NORMALIZED);
	FFixed32 SP_ZERO_ANIMWEIGHT_THRESH(ZERO_ANIMWEIGHT_THRESH);

	FFixed32 SP_ACOSMAGICONE((double)-0.0187293);
	FFixed32 SP_ACOSMAGICTWO((double)0.0742610);
	FFixed32 SP_ACOSMAGICTHREE((double)0.2121144);

	FFixed32 SP_ATANMAGICONE((double)+7.2128853633444123e-03f);
	FFixed32 SP_ATANMAGICTWO((double)-3.5059680836411644e-02f);
	FFixed32 SP_ATANMAGICTHREE((double)+8.1675882859940430e-02f);
	FFixed32 SP_ATANMAGICFOUR((double)-1.3374657325451267e-01f);
	FFixed32 SP_ATANMAGICFIVE((double)+1.9856563505717162e-01f);
	FFixed32 SP_ATANMAGICSIX((double)-3.3324998579202170e-01f);

	lines.Add(FString::Printf(TEXT("FIXED 32")));
	lines.Add(FString::Printf(TEXT("UE_PI:										%i"), SP_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_SMALL_NUMBER:							%i"), SP_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_KINDA_SMALL_NUMBER:						%i"), SP_KINDA_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_BIG_NUMBER:								%i"), SP_BIG_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_EULERS_NUMBER:							%i"), SP_EULERS_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_GOLDEN_RATIO:							%i"), SP_GOLDEN_RATIO.Value));
	lines.Add(FString::Printf(TEXT("UE_FLOAT_NON_FRACTIONAL:					%i"), SP_FLOAT_NON_FRACTIONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_PI:								%i"), SP_DOUBLE_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_SMALL_NUMBER:						%i"), SP_DOUBLE_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_KINDA_SMALL_NUMBER:				%i"), SP_DOUBLE_KINDA_SMALL_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_BIG_NUMBER:						%i"), SP_DOUBLE_BIG_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_EULERS_NUMBER:					%i"), SP_DOUBLE_EULERS_NUMBER.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_GOLDEN_RATIO:						%i"), SP_DOUBLE_GOLDEN_RATIO.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_NON_FRACTIONAL:					%i"), SP_DOUBLE_NON_FRACTIONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_MAX_FLT:									%i"), SP_MAX_FLT.Value));
	lines.Add(FString::Printf(TEXT("UE_INV_PI:									%i"), SP_INV_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_HALF_PI:									%i"), SP_HALF_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_TWO_PI:									%i"), SP_TWO_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_PI_SQUARED:								%i"), SP_PI_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_INV_PI:							%i"), SP_DOUBLE_INV_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_HALF_PI:							%i"), SP_DOUBLE_HALF_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_TWO_PI:							%i"), SP_DOUBLE_TWO_PI.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_PI_SQUARED:						%i"), SP_DOUBLE_PI_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_SQRT_2:									%i"), SP_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_SQRT_3:									%i"), SP_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_INV_SQRT_2:								%i"), SP_INV_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_INV_SQRT_3:								%i"), SP_INV_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_HALF_SQRT_2:								%i"), SP_HALF_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_HALF_SQRT_3:								%i"), SP_HALF_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_SQRT_2:							%i"), SP_DOUBLE_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_SQRT_3:							%i"), SP_DOUBLE_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_INV_SQRT_2:						%i"), SP_DOUBLE_INV_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_INV_SQRT_3:						%i"), SP_DOUBLE_INV_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_HALF_SQRT_2:						%i"), SP_DOUBLE_HALF_SQRT_2.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_HALF_SQRT_3:						%i"), SP_DOUBLE_HALF_SQRT_3.Value));
	lines.Add(FString::Printf(TEXT("UE_DELTA:									%i"), SP_DELTA.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_DELTA:							%i"), SP_DOUBLE_DELTA.Value));
	lines.Add(FString::Printf(TEXT("UE_FLOAT_NORMAL_THRESH:						%i"), SP_FLOAT_NORMAL_THRESH.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_NORMAL_THRESH:					%i"), SP_DOUBLE_NORMAL_THRESH.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINT_ON_PLANE:					%i"), SP_THRESH_POINT_ON_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINT_ON_SIDE:					%i"), SP_THRESH_POINT_ON_SIDE.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINTS_ARE_SAME:					%i"), SP_THRESH_POINTS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_POINTS_ARE_NEAR:					%i"), SP_THRESH_POINTS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_NORMALS_ARE_SAME:					%i"), SP_THRESH_NORMALS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_UVS_ARE_SAME:						%i"), SP_THRESH_UVS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_VECTORS_ARE_NEAR:					%i"), SP_THRESH_VECTORS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_SPLIT_POLY_WITH_PLANE:			%i"), SP_THRESH_SPLIT_POLY_WITH_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_SPLIT_POLY_PRECISELY:				%i"), SP_THRESH_SPLIT_POLY_PRECISELY.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_ZERO_NORM_SQUARED:				%i"), SP_THRESH_ZERO_NORM_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_NORMALS_ARE_PARALLEL:				%i"), SP_THRESH_NORMALS_ARE_PARALLEL.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_NORMALS_ARE_ORTHOGONAL:			%i"), SP_THRESH_NORMALS_ARE_ORTHOGONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_VECTOR_NORMALIZED:				%i"), SP_THRESH_VECTOR_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("UE_THRESH_QUAT_NORMALIZED:					%i"), SP_THRESH_QUAT_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINT_ON_PLANE:			%i"), SP_DOUBLE_THRESH_POINT_ON_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINT_ON_SIDE:				%i"), SP_DOUBLE_THRESH_POINT_ON_SIDE.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINTS_ARE_SAME:			%i"), SP_DOUBLE_THRESH_POINTS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_POINTS_ARE_NEAR:			%i"), SP_DOUBLE_THRESH_POINTS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_NORMALS_ARE_SAME:			%i"), SP_DOUBLE_THRESH_NORMALS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_UVS_ARE_SAME:				%i"), SP_DOUBLE_THRESH_UVS_ARE_SAME.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_VECTORS_ARE_NEAR:			%i"), SP_DOUBLE_THRESH_VECTORS_ARE_NEAR.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE:		%i"), SP_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_SPLIT_POLY_PRECISELY:		%i"), SP_DOUBLE_THRESH_SPLIT_POLY_PRECISELY.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_ZERO_NORM_SQUARED:			%i"), SP_DOUBLE_THRESH_ZERO_NORM_SQUARED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_NORMALS_ARE_PARALLEL:		%i"), SP_DOUBLE_THRESH_NORMALS_ARE_PARALLEL.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL:	%i"), SP_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_VECTOR_NORMALIZED:			%i"), SP_DOUBLE_THRESH_VECTOR_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("UE_DOUBLE_THRESH_QUAT_NORMALIZED:			%i"), SP_DOUBLE_THRESH_QUAT_NORMALIZED.Value));
	lines.Add(FString::Printf(TEXT("ZERO_ANIMWEIGHT_THRESH:						%i"), SP_ZERO_ANIMWEIGHT_THRESH.Value));
	lines.Add(FString::Printf(TEXT("ACOSMAGICONE:								%i"), SP_ACOSMAGICONE.Value));
	lines.Add(FString::Printf(TEXT("ACOSMAGICTWO:								%i"), SP_ACOSMAGICTWO.Value));
	lines.Add(FString::Printf(TEXT("ACOSMAGICTHREE:								%i"), SP_ACOSMAGICTHREE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICONE:								%i"), SP_ATANMAGICONE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICTWO:								%i"), SP_ATANMAGICTWO.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICTHREE:								%i"), SP_ATANMAGICTHREE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICFOUR:								%i"), SP_ATANMAGICFOUR.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICFIVE:								%i"), SP_ATANMAGICFIVE.Value));
	lines.Add(FString::Printf(TEXT("ATANMAGICSIX:								%i"), SP_ATANMAGICSIX.Value));
	FFileHelper::SaveStringArrayToFile(lines, TEXT("C:/Users/User/Documents/constants.txt"));

}

void ALockstepTestCharacter::DumpAtanAsFixed()
{
	TArray<FString> lines;
	TArray<FString> debuglines;
	lines.Add(FString::Printf(TEXT("FIXED 64")));
	debuglines.Add(FString::Printf(TEXT("FIXED 64")));
	for (FFixed64 atan64 = -FixedPoint::Constants::Fixed64::HalfPi; atan64 <= FixedPoint::Constants::Fixed64::HalfPi; atan64.Value++)
	{
		FFixed64 result = (FFixed64)FMath::Atan((double)atan64);
		lines.Add(FString::Printf(TEXT("FFixed64::MakeFromRawInt(%i);"), result.Value));
		debuglines.Add(FString::Printf(TEXT("%f		:	%f"), (double)atan64, (double)result));
	}
	FFileHelper::SaveStringArrayToFile(lines, TEXT("C:/Users/User/Documents/atanvaluesfixed.txt"));
	FFileHelper::SaveStringArrayToFile(debuglines, TEXT("C:/Users/User/Documents/atanvaluesfixeddebug.txt"));
}

void ALockstepTestCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALockstepTestCharacter, testfixednumber);
}