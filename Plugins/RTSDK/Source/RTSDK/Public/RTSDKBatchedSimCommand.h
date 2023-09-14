// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "MassCommands.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKScriptExecutionContext.h"

template <typename... TArgs>
struct FRTSDKBroadcastUnitOnCollidedCommand: public FMassBatchedEntityCommand
{
	using Super = FMassBatchedEntityCommand;

	FRTSDKBroadcastUnitOnCollidedCommand() 
	{
		OperationType = EMassCommandOperationType::None;
	}

	void Add(FMassEntityHandle Entity, URTSDKUnitComponent* Unit, FHitResult Hit)
	{
		Super::Add(Entity);
		TargetUnitComponents.Add(Unit);
		TriggeringHitResults.Add(Hit);
	}

	TArray<TWeakObjectPtr<URTSDKUnitComponent>> TargetUnitComponents;
	TArray<FHitResult> TriggeringHitResults;

	virtual void Execute(FMassEntityManager& System) const override
	{
		if (TargetUnitComponents.Num() <= 0)
		{
			return;
		}
		URTSDKGameSimSubsystem* sim = TargetUnitComponents[0]->OwningSim;
		FMassCommandBuffer& defer = sim->StartScriptCallingMode();
		FRTSDKScriptExecutionContext context(&defer);
		for (int32 i = 0; i < TargetUnitComponents.Num(); i++)
		{
			TargetUnitComponents[i]->OnUnitCollided.Broadcast(context, TriggeringHitResults[i]);
		}
		sim->EndScriptCallingMode();
	}
};
