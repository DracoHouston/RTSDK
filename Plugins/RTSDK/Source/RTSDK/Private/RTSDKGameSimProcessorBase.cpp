// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameSimProcessorBase.h"
#include "RTSDKGameSimSubsystem.h"
#include "MassSignalSubsystem.h"

bool URTSDKGameSimProcessorBase::AutoIncludeInSimPipeline()
{
	return true;
}

bool URTSDKGameSimProcessorBase::ShouldShowUpInSimSettings()
{
	return true;
}

void URTSDKGameSimProcessorBase::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);

	SimSubsystem = GetWorld()->GetSubsystem<URTSDKGameSimSubsystem>();
	MassSignalSubsystem = GetWorld()->GetSubsystem<UMassSignalSubsystem>();
}

bool URTSDKModGameSimProcessorBase::AutoIncludeInSimPipeline()
{
	return false;
}

bool URTSDKModGameSimProcessorBase::ShouldShowUpInSimSettings()
{
	return false;
}

bool URTSDKGameSimSignalProcessorBase::AutoIncludeInSimPipeline()
{
	return true;
}

bool URTSDKGameSimSignalProcessorBase::ShouldShowUpInSimSettings()
{
	return true;
}

void URTSDKGameSimSignalProcessorBase::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);

	SimSubsystem = GetWorld()->GetSubsystem<URTSDKGameSimSubsystem>();
	MassSignalSubsystem = GetWorld()->GetSubsystem<UMassSignalSubsystem>();
}

bool URTSDKModGameSimSignalProcessorBase::AutoIncludeInSimPipeline()
{
	return false;
}

bool URTSDKModGameSimSignalProcessorBase::ShouldShowUpInSimSettings()
{
	return false;
}
