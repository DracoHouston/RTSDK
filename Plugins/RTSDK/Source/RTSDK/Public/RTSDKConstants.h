// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#if RTSDK_USE_FIXED_POINT == 0
#include "DeterministicFloatTypes.h"
using FRTSMath = FDeterministicFloatMath;
using FRTSNumber64 =	FDeterministicNumber64;
using FRTSNumber32 =	FDeterministicNumber32;
using FRTSVector64 =	FDeterministicVector64;
//using FRTSVector32 =	FVector3f;
using FRTSQuat64 =		FDeterministicQuat64;
using FRTSRotator64 = FDeterministicRotator64;
//using FRTSQuat32 =		FQuat4f;
using FRTSTransform64 =	FDeterministicTransform64;
//using FRTSTransform32 = FTransform3f;
#else
#include "FixedPointTypes.h"
using FRTSMath =		FFixedPointMath;
using FRTSNumber64 =	FFixed64;
using FRTSNumber32 =	FFixed32;
using FRTSVector64 =	FFixedVector64;
//using FRTSVector32 =	FFixedVector32;
using FRTSQuat64 =		FFixedQuat64;
using FRTSRotator64 = FFixedRotator64;
//using FRTSQuat32 =		FFixedQuat32;
using FRTSTransform64 = FFixedTransform64;
//using FRTSTransform32 = FFixedTransform32;
#endif

namespace RTSDK
{
	const FName SimProcessingPhaseName = FName(TEXT("RTSDKGameSim"));
	const FName FallingUnitComponentTag = FName(TEXT("FallingUnitComponentTag"));
	const FString SharedUserSettingsSaveGameSlotName = TEXT("SharedUserSettings");
}

//Constants for processor group names, for execution order.
// Note for custom movement systems:
//For more than 1 game specific custom movement system, set up your own group, 
//execute after all RTSDKMovement____ groups, as well as RTSDKPreMovement group.
//execute before RTSDKPostMovement.
//for a single custom movement system, use the RTSDKMovementCustomRotationCommit 
//and RTSDKMovementCustomLocationCommit groups.
//this order will be respected by RTSDK processors

namespace UE::Mass::ProcessorGroupNames
{
	//Group for async processors that run before movement occurs
	const FName RTSDKPreMovement = FName(TEXT("RTSDKPreMovement"));
	//Group for async processors that find the desired position of units
	const FName RTSDKMovementPositionalDirectors = FName(TEXT("RTSDKMovementPositionalDirectors"));
	//Group for async processors that adjust the desired position of units to the best unrestricted position
	const FName RTSDKMovementPositionalRestrictors = FName(TEXT("RTSDKMovementPositionalRestrictors"));
	//Group for async processors that find the desired velocity based on the desired position of units
	const FName RTSDKMovementVelocityDirectors = FName(TEXT("RTSDKMovementVelocityDirectors"));
	//Group for async processors that adjust the desired velocity of units to the best unrestricted velocity
	const FName RTSDKMovementVelocityRestrictors = FName(TEXT("RTSDKMovementVelocityRestrictors"));
	//Group for async processors to perform last moment checks on movement values
	const FName RTSDKMovementPostVelocity = FName(TEXT("RTSDKMovementPostVelocity"));
	//Group for 'Simple 2D' movement system to perform the commitment of new rotations, with collision, game thread only
	const FName RTSDKMovementSimple2DRotationCommit = FName(TEXT("RTSDKMovementSimple2DRotationCommit"));
	//Group for 'Simple 2D' movement system to perform the commitment of new locations, with collision, game thread only
	const FName RTSDKMovementSimple2DLocationCommit = FName(TEXT("RTSDKMovementSimple2DLocationCommit"));
	//Group for 'Simple 3D' movement system to perform the commitment of new rotations, with collision, game thread only
	const FName RTSDKMovementSimple3DRotationCommit = FName(TEXT("RTSDKMovementSimple3DRotationCommit"));
	//Group for 'Simple 3D' movement system to perform the commitment of new locations, with collision, game thread only
	const FName RTSDKMovementSimple3DLocationCommit = FName(TEXT("RTSDKMovementSimple3DLocationCommit"));
	//Group for 'Complex 3D' movement system to perform the commitment of new rotations, with collision, game thread only
	const FName RTSDKMovementComplexRotationCommit = FName(TEXT("RTSDKMovementComplexRotationCommit"));
	//Group for 'Complex 3D' movement system to perform the commitment of new locations, with collision, game thread only
	const FName RTSDKMovementComplexLocationCommit = FName(TEXT("RTSDKMovementComplexLocationCommit"));
	//Group for a single game specific custom movement system to perform the commitment of new rotations, with collision, no thread restriction
	const FName RTSDKMovementCustomRotationCommit = FName(TEXT("RTSDKMovementCustomRotationCommit"));
	//Group for a single game specific custom movement system to perform the commitment of new locations, with collision, no thread restriction
	const FName RTSDKMovementCustomLocationCommit = FName(TEXT("RTSDKMovementCustomLocationCommit"));
	//Group for async processors that run after movement occurs
	const FName RTSDKPostMovement = FName(TEXT("RTSDKPostMovement"));
	//Group for game thread processors that run in the standard Mass phases to perform interpolation of the RTSDKVisRootComponent's transform, don't use with game sim processors
	const FName RTSDKVisInterpolation = FName(TEXT("RTSDKVisInterpolation"));
}

namespace UE::Mass::Signals
{
	const FName RTSDKUnitGravityChanged = FName(TEXT("RTSDKUnitGravityChanged"));
	const FName RTSDKUnitHasVelocity = FName(TEXT("RTSDKUnitHasVelocity"));
	const FName RTSDKUnitHasAngularVelocity = FName(TEXT("RTSDKUnitHasAngularVelocity"));
}

//game sim processor execution order macros
//Putting these in the constructor of a processor will set:
//'execution flags' to 'all'
//the exec group to the group named in the macro
//all execute before and after groups respective of the named group
//auto register with processing phases and can show up in settings to false
//it will not set the requires game thread flag, set that yourself

//RTSDKPreMovement Group
#define RTSDK_PROCESSOR_EXEC_ORDER_PRE_MOVEMENT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKPreMovement; \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementPositionalDirectors Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_POSITIONAL_DIRECTORS ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementPositionalRestrictors Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_POSITIONAL_RESTRICTORS ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementVelocityDirectors Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_VELOCITY_DIRECTORS ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false;\
bCanShowUpInSettings = false;

//RTSDKMovementVelocityRestrictors Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_VELOCITY_RESTRICTORS ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementPostVelocity Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_POST_VELOCITY ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementSimple2DRotationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_SIMPLE2D_ROTATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementSimple2DLocationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_SIMPLE2D_LOCATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementSimple3DRotationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_SIMPLE3D_ROTATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementSimple3DLocationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_SIMPLE3D_LOCATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementComplexRotationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_COMPLEX_ROTATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementComplexLocationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_COMPLEX_LOCATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementCustomRotationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_CUSTOM_ROTATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKMovementCustomLocationCommit Group
#define RTSDK_PROCESSOR_EXEC_ORDER_MOVEMENT_CUSTOM_LOCATION_COMMIT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::RTSDKPostMovement); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;

//RTSDKPostMovement Group
#define RTSDK_PROCESSOR_EXEC_ORDER_POST_MOVEMENT ExecutionFlags = (int32)EProcessorExecutionFlags::All; \
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::RTSDKPostMovement; \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKPreMovement); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPositionalRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityDirectors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementVelocityRestrictors); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementPostVelocity); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple2DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementSimple3DLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementComplexLocationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomRotationCommit); \
ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::RTSDKMovementCustomLocationCommit); \
bAutoRegisterWithProcessingPhases = false; \
bCanShowUpInSettings = false;
