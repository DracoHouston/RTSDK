// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "RTSDKGameSimProcessorInterface.h"
#include "RTSDKGameSimProcessorBase.generated.h"

class URTSDKGameSimSubsystem;
class UMassSignalSubsystem;

/**
 * Base class for game sim processors. These are coordinated by RTSGameSimSubsystem.
 * Has a pointer to the SimSubsystem, set if you chain Super::Initialize.
 */
UCLASS(abstract)
class RTSDK_API URTSDKGameSimProcessorBase : public UMassProcessor, public IRTSDKGameSimProcessorInterface
{
	GENERATED_BODY()
public:
	URTSDKGameSimProcessorBase() {}

	virtual void Initialize(UObject& Owner) override;
protected:
	URTSDKGameSimSubsystem* SimSubsystem;
	UMassSignalSubsystem* MassSignalSubsystem;
};
