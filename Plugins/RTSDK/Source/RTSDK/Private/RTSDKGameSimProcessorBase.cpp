// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameSimProcessorBase.h"
#include "RTSDKGameSimSubsystem.h"
#include "MassSignalSubsystem.h"

void URTSDKGameSimProcessorBase::Initialize(UObject& Owner)
{
	SimSubsystem = GetWorld()->GetSubsystem<URTSDKGameSimSubsystem>();
	MassSignalSubsystem = GetWorld()->GetSubsystem<UMassSignalSubsystem>();
}
