// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Engine/NetSerialization.h"
#include "RTSDKConstants.h"
#include "FixedPointTypes.h"
#include "RTSDKPlayerCommand.h"
#include "RTSDKPlayerState.generated.h"



/**
 * 
 */
UCLASS()
class RTSDK_API ARTSDKPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
		int32 PlayerIndex;

	UPROPERTY(Replicated)
		bool bIsReady;
};
