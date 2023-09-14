// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "RTSDKConstants.h"
#include "MassCommands.h"
#include "RTSDKFragments.h"


template <typename... TArgs>
struct FRTSDKSetUnitInputScriptCommand : public FMassBatchedEntityCommand
{
	using Super = FMassBatchedEntityCommand;

	FRTSDKSetUnitInputScriptCommand()
	{
		OperationType = EMassCommandOperationType::None;
	}

	void Add(FMassEntityHandle Entity, FRTSVector64 Direction)
	{
		Super::Add(Entity);
		NewDirections.Add(Direction);
	}

	TArray<FRTSVector64> NewDirections;

	virtual void Execute(FMassEntityManager& System) const override
	{
		for (int32 i = 0; i < NewDirections.Num(); i++)
		{
			System.GetFragmentDataChecked<FRTSMovementInputFragment>(TargetEntities[i]).Input = NewDirections[i];
			System.GetFragmentDataChecked<FRTSLookInputFragment>(TargetEntities[i]).Input.Yaw = NewDirections[i].ToOrientationRotator().Yaw;
		}
	}
};