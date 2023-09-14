// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "RTSDKSimCompositeProcessor.generated.h"

/**
 * Composite processor that is run in order to run game sim processors on a fixed timestep, with catchup
 * See URTSDKGameSimSubsystem, which is responsible for running this, and URTSDKMassModuleSettings, which 
 * auto discovers rts game sim interface using processors.
 */
UCLASS()
class RTSDK_API URTSDKSimCompositeProcessor : public UMassCompositeProcessor
{
	GENERATED_BODY()
	
};
