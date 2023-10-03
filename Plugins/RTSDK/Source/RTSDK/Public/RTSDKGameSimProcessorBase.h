// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassSignalProcessorBase.h"
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

	virtual bool AutoIncludeInSimPipeline() override;
	virtual bool ShouldShowUpInSimSettings() override;

	virtual void Initialize(UObject& Owner) override;
protected:
	URTSDKGameSimSubsystem* SimSubsystem;
	UMassSignalSubsystem* MassSignalSubsystem;
};

/**
 * Base class for game sim processors. These are coordinated by RTSGameSimSubsystem.
 * Has a pointer to the SimSubsystem, set if you chain Super::Initialize.
 */
UCLASS(abstract)
class RTSDK_API URTSDKModGameSimProcessorBase : public URTSDKGameSimProcessorBase
{
	GENERATED_BODY()
public:
	URTSDKModGameSimProcessorBase() {}

	virtual bool AutoIncludeInSimPipeline() override;
	virtual bool ShouldShowUpInSimSettings() override;

	//virtual void Initialize(UObject& Owner) override;
};


/**
 * Base class for game sim signal processors. These are coordinated by RTSGameSimSubsystem.
 * Has a pointer to the SimSubsystem, set if you chain Super::Initialize.
 */
UCLASS(abstract)
class RTSDK_API URTSDKGameSimSignalProcessorBase : public UMassSignalProcessorBase, public IRTSDKGameSimProcessorInterface
{
	GENERATED_BODY()
public:
	URTSDKGameSimSignalProcessorBase() {}

	virtual bool AutoIncludeInSimPipeline() override;
	virtual bool ShouldShowUpInSimSettings() override;

	virtual void Initialize(UObject& Owner) override;
protected:
	URTSDKGameSimSubsystem* SimSubsystem;
	UMassSignalSubsystem* MassSignalSubsystem;
};

/**
 * Base class for game sim signal processors inside mods. These are coordinated by RTSGameSimSubsystem.
 * Has a pointer to the SimSubsystem, set if you chain Super::Initialize.
 */
UCLASS(abstract)
class RTSDK_API URTSDKModGameSimSignalProcessorBase : public URTSDKGameSimSignalProcessorBase
{
	GENERATED_BODY()
public:
	URTSDKModGameSimSignalProcessorBase() {}

	virtual bool AutoIncludeInSimPipeline() override;
	virtual bool ShouldShowUpInSimSettings() override;
};
