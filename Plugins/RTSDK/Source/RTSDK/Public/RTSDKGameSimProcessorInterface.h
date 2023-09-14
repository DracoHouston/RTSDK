// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSDKGameSimProcessorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSDKGameSimProcessorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for auto detection of a RTS 'game sim' processor that runs
 * during the rts game sim fixed timestep ticks
 */
class RTSDK_API IRTSDKGameSimProcessorInterface
{
	GENERATED_BODY()

public:

	/**
	* If this returns true RTSDKMassModuleSettings will autoinclude this in the sim processor pipeline
	*/
	bool virtual AutoIncludeInSimPipeline()
	{
		return true;
	}

	/**
	* If this returns true RTSDKMassModuleSettings will include this in the sim processor CDO list
	*/
	bool virtual ShouldShowUpInSimSettings()
	{
		return true;
	}
};
