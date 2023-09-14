// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RTSDKGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSDK_API ARTSDKGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

		UPROPERTY(replicated)
		bool bRTSDKMatchStarted;
};
