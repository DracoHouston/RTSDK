// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassEntityTypes.h"
//#include "LockstepTypes.generated.h"

//Constants for signal names
//namespace UE::Mass::Signals
//{
//
//}

//Constants for processor group names, for execution order
namespace UE::Mass::ProcessorGroupNames
{
	const FName LockstepMovement = FName(TEXT("LockstepMovement"));
	const FName LockstepInterpolation = FName(TEXT("LockstepInterpolation"));
}

